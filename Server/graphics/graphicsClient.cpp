#include "graphicsClient.h"
#include "oglUtil.h"
#include "mesh.h"
#include "loadCustom.h"
#include "Entity.h"
#include "PowerUp.h"
#include "Projectile.h"

namespace {
const unsigned int defaultWindowWidth  = 800;
const unsigned int defaultWindowHeight = 600;

gx::graphicsEntity loadModel(const std::string& ModelPath,
    gx::Mesh::length_t height, bool flipUVs = false) {
  return gx::Mesh(ModelPath,height,flipUVs).entityData;
}

std::string configModelName(std::string s) {
  return "models/" + ConfigManager::configMap["model-" + s];
}

std::vector<gx::graphicsEntity> staticModels() {

  //auto modelBadGuy     = loadModel(configModelName("badguy"),Player::playerDepth,true);
  auto modelWeapon     = loadModel(configModelName("weapon"),Weapon::weaponDepth,true);
  auto modelProjectile = loadModel(configModelName("projectile"),Projectile::projDepth,true);
  auto modelPowerUp    = loadModel(configModelName("powerup"),PowerUp::powerUpDepth,true);
  auto modelWall       = loadModel(configModelName("wall"),10);
  auto modelPlayer     = loadModel(configModelName("goodguy"),Player::playerDepth,true);
  //auto modelTriton     = loadModel(configModelName("triton"),Player::playerDepth,true);

  auto cubes = gx::loadCube();
  auto skybox = gx::loadSkybox();
  auto ground = gx::loadGround(2000.0f, 0.0f, "models/floor.jpg");
  std::vector<gx::graphicsEntity> entitiesData;

  entitiesData.push_back(std::move(skybox));  //ignore
  entitiesData.push_back(std::move(modelPlayer)); //player
  entitiesData.push_back(std::move(modelWall));  //wall
  entitiesData.push_back(std::move(modelProjectile)); //projectile
  entitiesData.push_back(std::move(modelWeapon)); //weapon
  entitiesData.push_back(std::move(modelPowerUp)); //powerup
 // entitiesData.push_back(std::move(modelTriton));
  entitiesData.push_back(std::move(ground));  //ground
  //entitiesData.push_back(std::move(modelTriton));

  entitiesData.insert(entitiesData.end(),std::make_move_iterator(cubes.begin()),
                                         std::make_move_iterator(cubes.end())); 
  return entitiesData;
}

std::vector<gx::graphicsEntity> dynamicModels() {
  // MODEL LOADING
  auto modelPlayer = loadModel(configModelName("goodguy"),Player::playerDepth,true);
 // auto modelWall =  loadModel(configModelName("ninja"),10);
    //setup drawing data
  std::vector<gx::graphicsEntity> entitiesData;
  entitiesData.push_back(std::move(modelPlayer));
  return entitiesData;
}

std::vector<gx::particleEntity> particlesData() {
  gx::material mat(gx::Texture(GL_TEXTURE_2D,"models/Flame_1.jpg"),gx::vector4f(0.75,0,0));
  std::vector<gx::particleEntity> ret;
  ret.push_back(gx::particleEntity(std::move(mat)));
  return ret;
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
  const elem_t ratio     = static_cast<elem_t>(w) / static_cast<elem_t>(h);
  const elem_t nearPlane = 1.0f;
  const elem_t farPlane  = 3000.0f;
  // RenderWindow automatically sets the viewport on a resize
  // in Linux but not in windows so we have to do it here
  glViewport(0, 0, w, h);
  debugout << "glViewport(0, 0, " << w << ", " << h << ");" << endl;
  this->window.setView(sf::View(sf::FloatRect(0,0,static_cast<float>(w),static_cast<float>(h))));
  this->display.setProjection(fov,ratio,nearPlane,farPlane);
}

std::vector<gx::uniform::block*> gx::graphicsClient::uniforms() {
  std::vector<gx::uniform::block*> ret;
  ret.push_back(&this->display.storage());
  ret.push_back(&this->lights.storage());
  return ret;
}
// create the window
//make sure this is above al opengl objects so that the desctructor is called
//last so we have an opengl context for destructors
gx::graphicsClient::graphicsClient():
    window(sf::VideoMode(ConfigManager::windowWidth(), ConfigManager::windowHeight()), "DrChao",
    (ConfigManager::fullscreen() ? sf::Style::Fullscreen : sf::Style::Default),
    sf::ContextSettings(24,0,ConfigManager::antiAliasingLevel())),
    //glew needs to be called here, after window, before anything else
    glewStatus(initGlew()),
    userInput(),
    lights(gx::vector4f(1,1,1),0.1,0.1,0.0f),
    display(),
    entities(staticModels(),uniforms()),
    staticEntities(staticModels(),uniforms()),
    animatedDrawer(dynamicModels(),uniforms()),
    skyboxDrawer(display.storage()),
    particles(particlesData(),std::vector<uniform::block*>(1,&(display.storage()))),
    Hud(),Lobby(), Score(ConfigManager::numPlayers()),
    playerDirection(0.0, 1.0,0.0),//will be changed by init packet
    playerStartDirection(0.0, 1.0,0.0),
    playerStartRight(playerStartDirection.y,-playerStartDirection.x,
                     playerStartDirection.z),
    playerPosition(0.0, 0.0, 0.0),//will be changed by init packet
    fpsClock(), fpsFrames(0) {
  sf::ContextSettings settings = this->window.getSettings();

  debugout << "depth bits:" << settings.depthBits << endl;
  debugout << "stencil bits:" << settings.stencilBits << endl;
  debugout << "antialiasing level:" << settings.antialiasingLevel << endl;
  debugout << "version:" << settings.majorVersion << "." << settings.minorVersion << endl;

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

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  //glBlendFunc(GL_DST_COLOR, GL_ONE);

  this->setCamera();
  this->userInput.setUpMouse();

  this->reshape(ConfigManager::windowWidth(), ConfigManager::windowHeight());
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
  
  if(this->userInput.getJump()) {
    action.jump = true;
  }
  action.movement = this->userInput.movePlayer();
  auto dir = this->playerDirection;
  action.updated = this->userInput.getUpdated();
  action.facingDirection = dir;
  action.attackMelee = this->userInput.fire2();
  action.attackRange = this->userInput.fire1();
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
  this->staticEntities.draw();
  this->entities.draw();
  this->animatedDrawer.draw();
  this->skyboxDrawer.draw();

  //glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  this->particles.draw();
 	glDepthMask(GL_TRUE);
  //glDisable(GL_BLEND);
 
  //render sfml please don't comment or uncomment anything from the following
  //block
  glBindVertexArray(0);
  debugout << "Bound 0 draw loop" << endl;
  this->window.pushGLStates();
  //glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //SFML graphics
  this->Hud.draw(window); 
  if (this->userInput.drawS()) {
    this->Score.draw(window);
  }
  //end of SFML graphics
  this->window.popGLStates();
  //glDisableClientState(GL_VERTEX_ARRAY);
  //glDisableClientState(GL_COLOR_ARRAY);
  //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  // end the current frame (internally swaps the front and back buffers)
  this->window.display();
  //fps calc
  this->fpsFrames++;
  if(this->fpsClock.getElapsedTime().asSeconds() >= 1) {
    std::cout << "fps: " << this->fpsFrames << std::endl;
    this->fpsFrames = 0;
    this->fpsClock.restart();
  }
}

void gx::graphicsClient::updatePosition(vector4f pos) {
  this->playerPosition = pos;
  this->setCamera();
}

void gx::graphicsClient::updateDirection(vector3f dir) {
  this->playerDirection = dir;
  this->setCamera();
}

int aniFrame = 0;

void gx::graphicsClient::clearEntities() {
  lights.clear();

  this->entities.reset();
  this->animatedDrawer.reset();
  this->particles.reset();
}

void gx::graphicsClient::addEntity(Entity* ent) {
  const auto& entity = *ent;
  const auto& type = entity.getType();
  if(type == WEAPON) {
	particleDrawer::instanceData inst;
    inst.position = vector4f(0,0,0) + entity.getPosition();
    inst.type = 0;
    this->particles.addInstance(inst);
  }

  if(false) { //TODO: change back to type == PLAYER
    dynamicDrawer::instanceData inst;
    inst.scale = 1;
    inst. pos = entity.getPosition();
    inst.dirY = entity.getDirection();
    inst.type = 0; //TODO: somehow set this based on type but it can't be absolute type?
    inst.animation = 0; //TODO: select animation based on context
    ++aniFrame;
    aniFrame %= 240;
    inst.timePosition = static_cast<double>(aniFrame) / 1200.0;
    this->animatedDrawer.addInstance(inst);
  } else {
    staticDrawer::instanceData inst;
    inst.scale = 1;
    inst. pos = entity.getPosition();
    inst.dirY = entity.getDirection();
    inst.type = entity.getType();
    this->entities.addInstance(inst);
  }
}

void gx::graphicsClient::addEntity(Projectile* ent) {
  const auto& entity = *ent;
  {
	  particleDrawer::instanceData inst;
    inst.position = vector4f(0,0,0) + entity.getPosition();
    inst.type = 0;
    this->particles.addInstance(inst);
  }
  lights.addLight(vector4f(0,0,0) + entity.getPosition());
  staticDrawer::instanceData inst;
  inst.scale = ProjInfo[entity.getMagicType()].size;
  inst. pos = entity.getPosition();
  inst.dirY = entity.getDirection();
  inst.type = entity.getType();
  this->entities.addInstance(inst);
}

void gx::graphicsClient::updateHUD(Player & player) {
  this->Hud.updateHUD(player);
}

void gx::graphicsClient::drawLobby() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glBindVertexArray(0);
  window.pushGLStates();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  this->userInput.handle(this->window);
  if(this->userInput.resizedWindow()) {
    reshape(this->userInput.windowWidth(),this->userInput.windowHeight());
  }
  this->Lobby.handleInput(this->userInput);
  this->Lobby.drawLobby(this->window);
  window.popGLStates(); 
  window.display();
}
void gx::graphicsClient::setStaticEntities(std::vector<StaticEntity*> e) {
  for(int i = 0; i < e.size() ;i++) {
    staticDrawer::instanceData inst;
    inst.scale = 1;
    inst.pos = e[i]->getPosition();
    inst.dirY = e[i]->getDirection();
    inst.type = e[i]->static_entity_type;
    inst.scale = e[i]->scale;
    this->staticEntities.addInstance(inst);
  }
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

void gx::graphicsClient::setMinotaur(unsigned int playerid)
{
  Score.setMinotaurId(playerid);
}

void gx::graphicsClient::updateScores(std::vector<int> & pwins,
                                      std::vector<int> & pkills,
                                      std::vector<bool> & pdead) {
  Score.updateScores(pwins,pkills, pdead);
}

void gx::graphicsClient::updateLobby(std::vector<std::pair<int,bool>> & playerStatus ) {
  Lobby.updateLobby(playerStatus);
}

void gx::graphicsClient::updateHUDTimer(float timer) {
  this->Hud.updateHUDTimer(timer);
}

std::string gx::graphicsClient::getInputText() const {
  return this->Lobby.getInputText();
}

void gx::graphicsClient::setConnected(bool connected) {
  this->Lobby.setConnected(connected);
}
void gx::graphicsClient::updateNames(std::vector<std::string> & names) {
  this->Score.updateNames(names);
}
