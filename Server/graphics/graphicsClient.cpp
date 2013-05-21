#include "graphicsClient.h"
#include "oglUtil.h"
#include "mesh.h"
#include "loadCube.h"
#include "Entity.h"

namespace {
const unsigned int defaultWindowWidth  = 800;
const unsigned int defaultWindowHeight = 600;

gx::dynamicEntity loadModel(const std::string& ModelPath) {
	//just do the first one unless kangh has a model with more
	return std::move(gx::Mesh(ModelPath,5).m_Entries[0].entitiesData);
}

std::vector<gx::dynamicEntity> entitiesData() {
	// MODEL LOADING
  auto model_import  = loadModel("models/boblampclean.md5mesh");
  //auto model_import  = loadModel("models/weird_orange_thing.dae");
  auto model_import2 = loadModel("models/Test_Run.dae");
  auto wallImport    = loadModel("models/wall.dae");

    //setup drawing data
  std::vector<gx::dynamicEntity> entitiesData;
  auto cubes = gx::loadCube();
  auto skybox = gx::loadSkybox();
  entitiesData.push_back(std::move(skybox));
  entitiesData.push_back(std::move(model_import2));
  entitiesData.push_back(std::move(wallImport));
  entitiesData.insert(entitiesData.end(),std::make_move_iterator(cubes.begin()),
                                         std::make_move_iterator(cubes.end()));
  entitiesData.push_back(std::move(model_import));
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
    entities(entitiesData(),uniforms()),
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
  action.facingDirection = dir;
  action.attackMelee = fire1();
  action.attackRange = fire2();
  action.pickup = this->userInput.pickUp();
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

void gx::graphicsClient::updateEntities(std::vector<Entity*> data) {
  this->entities.reset();

  for(auto entityP = data.begin(); entityP != data.end(); ++entityP) {
    const auto& entity = **entityP;
    typename staticDrawer::instanceData inst;
     inst.pos = entity.getPosition();
    inst.dirY = entity.getDirection();
    inst.type = entity.getType();
    entities.addInstance(inst);
  }
}
