#include "graphicsClient.h"
#include "oglUtil.h"
#include "mesh.h"
#include "loadCustom.h"
#include "Entity.h"

namespace {
const unsigned int defaultWindowWidth  = 800;
const unsigned int defaultWindowHeight = 600;

gx::graphicsEntity loadModel(const std::string& ModelPath) {
  return gx::Mesh(ModelPath,5).entityData; //add std::move if this copies
}

std::vector<gx::graphicsEntity> staticModels() {
  auto modelJack   = loadModel("models/Badguy_texture.dae");
  auto modelWall   = loadModel("models/wall.dae");
  auto modelPlayer = loadModel("models/Test_Run.dae");
  auto cubes = gx::loadCube();
  auto skybox = gx::loadSkybox();
  std::vector<gx::graphicsEntity> entitiesData;
  entitiesData.push_back(std::move(skybox));
  entitiesData.push_back(std::move(modelPlayer));
  entitiesData.push_back(std::move(modelWall));
  entitiesData.insert(entitiesData.end(),std::make_move_iterator(cubes.begin()),
                                         std::make_move_iterator(cubes.end()));
  entitiesData.push_back(std::move(modelJack));
  return entitiesData;
}

std::vector<gx::graphicsEntity> dynamicModels() {
  // MODEL LOADING
  //auto modelTest   = loadModel("models/boblampclean.md5anim");
  auto modelPlayer = loadModel("models/cat.dae");

    //setup drawing data
  std::vector<gx::graphicsEntity> entitiesData;
  entitiesData.push_back(std::move(modelPlayer));
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
  //add the direction vector to the player's position to get the position to
  //look at
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
  //this->window.setView(w,h); //maybe better?
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
//make sure this is above al opengl objects so that the desctructor is called
//last so we have an opengl context for destructors
gx::graphicsClient::graphicsClient():
    window(sf::VideoMode(defaultWindowWidth, defaultWindowHeight),
           "DrChao", sf::Style::Default),
    //glew needs to be called here, after window, before anything else
    glewStatus(initGlew()),
    userInput(),
    light1(gx::vector4f(1,1,1),0.5,0.5,0.05f),
    display(),
    entities(staticModels(),uniforms()),
    animatedDrawer(dynamicModels(),uniforms()),
	skyboxDrawer(this->display.storage()),
    playerDirection(0.0, 1.0,0.0),//change to result of init packet
    playerStartDirection(0.0, 1.0,0.0),//change to result of init packet
    playerStartRight(playerStartDirection.y,-playerStartDirection.x,
                     playerStartDirection.z),
    playerPosition(0.0, 0.0, 0.0),//change to the result of init packet
    fpsClock(), fpsFrames(0),Hud(),Lobby(), Score(ConfigManager::numPlayers()) {
  this->window.setVerticalSyncEnabled(false);
  this->window.setMouseCursorVisible(true);
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
  this->playerDirection =
    toBasis(playerStartRight,playerStartDirection,upDirection) * newdir;
  this->setCamera(); //after setting new player position and direction
  if(jumped()) {
    action.jump = true;
  }
  action.movement = this->userInput.movePlayer();
  auto dir = getDir();
  action.updated = this->userInput.getUpdated();
  action.facingDirection = dir;
  action.attackMelee = fire1();
  action.attackRange = fire2();
  action.pickup = this->userInput.pickUp();
  action.switchWeapon = this->userInput.switchW();
  return action;
}

void gx::graphicsClient::draw() {
    // clear the buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  gx::debugout << "glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT";
  gx::debugout << "| GL_STENCIL_BUFFER_BIT);" << gx::endl;
  
  // draw...
  entities.draw();
  animatedDrawer.draw();
  skyboxDrawer.draw();
  
  //render sfml please don't comment or uncomment anything from the following
  //block
  glBindVertexArray(0);
  debugout << "Bound 0 draw loop" << endl;
  window.pushGLStates();
  //glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //SFML graphics
  Hud.draw(window); 
  if (this->userInput.drawS())
    Score.draw(window);
  //end of SFML graphics
  window.popGLStates();
  //glDisableClientState(GL_VERTEX_ARRAY);
  //glDisableClientState(GL_COLOR_ARRAY);
  //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

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
int aniFrame = 0;
void gx::graphicsClient::updateEntities(std::vector<Entity*> data) {
  this->entities.reset();
  this->animatedDrawer.reset();

  for(auto entityP = data.begin(); entityP != data.end(); ++entityP) {
    const auto& entity = **entityP;
    const auto& type = entity.getType();
    if(type == PLAYER) { //TODO: change back to type == PLAYER
      dynamicDrawer::instanceData inst;
      inst. pos = entity.getPosition();
      inst.dirY = entity.getDirection();
      inst.type = 0; //TODO: somehow set this based on type but it can't be absolute type?
      inst.animation    = 0;
      ++aniFrame;
      aniFrame %= 80;
      inst.timePosition = aniFrame / 2;
      this->animatedDrawer.addInstance(inst);
    } else {
      staticDrawer::instanceData inst;
      inst. pos = entity.getPosition();
      inst.dirY = entity.getDirection();
      inst.type = entity.getType();
      this->entities.addInstance(inst);
    }
  }
}

void gx::graphicsClient::updateHUD(Player & player) {
  this->Hud.updateHUD(player);
}

void gx::graphicsClient::drawLobby() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glBindVertexArray(0);
  window.pushGLStates();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  this->Lobby.drawLobby(window);
  window.popGLStates(); 
  window.display();
}
void gx::graphicsClient::disableCursor() {
  this->window.setMouseCursorVisible(false);
}

void gx::graphicsClient::enableCursor() {
  this->window.setMouseCursorVisible(true);
}

bool gx::graphicsClient::gameStart() const {
  return (this->Lobby).getStart();
}

void gx::graphicsClient::gameEnd()
{
  Lobby.endGame();
}

void gx::graphicsClient::updateScores(std::vector<int> & pwins,
                                      std::vector<int> & pkills) {
  Score.updateScores(pwins,pkills);
}
