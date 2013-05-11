#include "graphicsClient.h"
#include <fstream>
#include <assimp/cimport.h> 
#include "oglUtil.h"
#include "mesh.h"
#include "loadCube.h"

namespace {
const unsigned int defaultWindowWidth  = 800;
const unsigned int defaultWindowHeight = 600;

std::string readFile(const std::string fileName) {
  std::ifstream vsFile(fileName);
  std::string fullSource = "";

  if(vsFile.is_open()) {
    std::string line;
    while(vsFile.good()) {
      getline(vsFile,line);
      fullSource += line + "\n";
    }
    vsFile.close();
  } else {
    std::cout << "error: cannot open file: " << fileName << std::endl;
  }
  return fullSource;
}

std::vector<gx::drawSet::vaoData_t> loadModel(const std::string& ModelPath) {
	gx::Mesh model(ModelPath);

	std::vector<gx::drawSet::vaoData_t> entities;
	//just do the first one until we get loading working
	entities.push_back(model.m_Entries[0].entitiesData);

	return entities;
}

std::vector<gx::drawSet::vaoData_t> entitiesData() {
	// MODEL LOADING
  std::vector<gx::drawSet::vaoData_t> model_import  = loadModel("models/weird_orange_thing.dae");
  std::vector<gx::drawSet::vaoData_t> model_import2 = loadModel("models/Model_rotate.dae");
  std::vector<gx::drawSet::vaoData_t> wallImport    = loadModel("models/wall.dae");

    //setup drawing data
  std::vector<gx::drawSet::vaoData_t> entitiesData;
  auto cubes = gx::loadCube();
  entitiesData.insert(entitiesData.end(),model_import2.begin(),model_import2.end());
  //entitiesData.insert(entitiesData.end(),model_import.begin(),model_import.end());
  entitiesData.insert(entitiesData.end(),wallImport.begin(),wallImport.end());
  entitiesData.insert(entitiesData.end(),cubes.begin(),cubes.end());
  return entitiesData;
}
//must call after window is initialized
GLenum initGlew() {
  //should glew be done per context?? if so, move to static method
  //and ensure that it is only called once
  GLenum glewErr = glewInit();
  if(GLEW_OK != glewErr) {
    std::cout << "error initializing GLEW!" << std::endl;
    exit(1);
  }
  return glewErr;
}
} //end unnamed namespace

const gx::vector3f gx::graphicsClient::upDirection(0.0, 0.0, 1.0);

void gx::graphicsClient::setCamera() {
  //add the direction vector to the player's position to get the position to look at
  this->display.setView(playerPosition,
                        playerDirection + playerPosition,
                        upDirection);
}

void gx::graphicsClient::reshape(unsigned int w, unsigned int h) {
  typedef displaySet::elem_t elem_t;
  const elem_t fov       = 53.13f;
  const elem_t ratio     = elem_t(w) / elem_t(h);
  const elem_t nearPlane = 1.0f;
  const elem_t farPlane  = 3000.0f;
  // adjust the viewport when the window is resized
  glViewport(0, 0, w, h);
  gx::debugout << "glViewport(0, 0, " << w << ", " << h << ");" << gx::endl;
  this->display.setProjection(fov,ratio,nearPlane,farPlane);
}

std::vector<gx::uniform::block*> gx::graphicsClient::uniforms() {
  std::vector<gx::uniform::block*> ret;
  ret.push_back(&this->display.storage());
  ret.push_back(&this->light1.storage());
  return ret;
}
// create the window
//make sure this is above all opengl objects so that the desctructor is called
//last so we have an opengl context for destructors
gx::graphicsClient::graphicsClient():
    window(sf::VideoMode(defaultWindowWidth, defaultWindowHeight),
           "DrChao", sf::Style::Default),
    glewStatus(initGlew()), //glew needs to be called here, after window, before anything else
    userInput(),
    light1(gx::vector4f(1,1,1),0.5,0.5,0.05f),
    display(),
    entities(readFile("shaders/default.vert"),readFile("shaders/default.frag"),
                       entitiesData(),uniforms()),
    playerDirection(0.0, 1.0,0.0),//change to result of init packet
    playerStartDirection(0.0, 1.0,0.0),//change to result of init packet
    playerStartRight(playerStartDirection.y,playerStartDirection.x,playerStartDirection.z),
    playerPosition(0.0, 0.0, 0.0),//change to the result of init packet
     fpsClock(), fpsFrames(0)                 {
  this->window.setVerticalSyncEnabled(false);
  this->window.setMouseCursorVisible(false);
  if(!this->window.setActive()) {
    std::cout << "error activating window" << std::endl;
    exit(1);
  }
  
  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glClearColor(1.0,1.0,1.0,1.0); //start with a white screen
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  light1.updatePosition(gx::vector4f( 0, 5, -10));

  this->setCamera();
  this->userInput.setUpMouse();
  this->reshape(defaultWindowWidth, defaultWindowHeight);
}

ClientGameTimeAction gx::graphicsClient::handleInput() {
  action.clear();
  this->userInput.handle(this->window);
  if(this->userInput.resizedWindow()) {
    reshape(this->userInput.windowWidth(),this->userInput.windowHeight());
  }
  auto newdir = this->userInput.turnPlayer();
  this->playerDirection = toBasis(playerStartRight,playerStartDirection,upDirection) * newdir;
  this->setCamera(); //after setting new player position and direction
  if(jumped()) {
    action.jump = true;
  }
  action.movement = getMovement();
  auto dir = getDir();
  action.updated = this->userInput.getUpdated();
  action.facingDirection = Direction(dir.x, dir.y, dir.z);
  action.attackMelee = fire1();
  action.attackRange = fire2();
  return action;
}

void gx::graphicsClient::draw() {
    // clear the buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  gx::debugout << "glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT";
  gx::debugout << "| GL_STENCIL_BUFFER_BIT);" << gx::endl;

    // draw...
	entities.draw();

  // end the current frame (internally swaps the front and back buffers)
  window.display();

  //fps calc
  fpsFrames++;
  if(fpsClock.getElapsedTime().asSeconds() >= 1) {
    std::cout << "fps: " << fpsFrames << std::endl;
    fpsFrames = 0;
    fpsClock.restart();
  }
}

void gx::graphicsClient::updatePosition(vector4f pos) {
  this->playerPosition = pos;
  this->setCamera();
}

void gx::graphicsClient::updateEntities(std::vector<graphicEntity> data) {
  this->entities.reset();

  for(auto entityP = data.begin(); entityP != data.end(); ++entityP) {
    const auto& entity = *entityP;
    entities.addEntity(entity.position, entity.direction, entity.type);
  }
}
