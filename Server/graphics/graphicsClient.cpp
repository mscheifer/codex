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

unsigned int projectileOffset[NUM_MAGIC];

unsigned int projectileBase;

std::vector<gx::graphicsEntity> initProjectileModels(unsigned int base) {
  projectileBase = base;

  auto modelIce   = loadModel(configModelName("ice"),Projectile::projDepth,true);
  auto modelFire  = loadModel(configModelName("thunder"),Projectile::projDepth,true);
  auto modelDark  = loadModel(configModelName("dark"),Projectile::projDepth,true);
  auto modelBasic = loadModel(configModelName("basic"),Projectile::projDepth,true);

  std::vector<gx::graphicsEntity> ret;
  ret.push_back(std::move(modelIce));
  projectileOffset[ICE1] = 0;
  projectileOffset[ICE2] = 0;
  projectileOffset[ICE3] = 0;
  ret.push_back(std::move(modelFire));
  projectileOffset[THU1] = 1;
  projectileOffset[THU2] = 1;
  projectileOffset[THU3] = 1;
  ret.push_back(std::move(modelDark));
  projectileOffset[G_IT] = 2;
  projectileOffset[G_FT] = 2;
  projectileOffset[G_FI] = 2;
  projectileOffset[G2]   = 2;
  projectileOffset[G_IT2]= 2;
  projectileOffset[G_FT2]= 2;
  projectileOffset[G_FI2]= 2;
  projectileOffset[G3]   = 2;
  ret.push_back(std::move(modelBasic));
  projectileOffset[B1]   = 3;

  return ret;
}

unsigned int playerBase;

std::vector<gx::graphicsEntity> initPlayerModels(unsigned int base) {
  playerBase = base;
  auto modelBadGuy     = loadModel(configModelName("badguy"),Player::playerDepth,true);
  auto modelPlayer     = loadModel(configModelName("goodguy"),Player::playerDepth,true);

  std::vector<gx::graphicsEntity> ret;
  ret.push_back(std::move(modelPlayer));
  ret.push_back(std::move(modelBadGuy));
  return ret;
}

unsigned int generalOffset[NUM_ENTITIES];

std::vector<gx::graphicsEntity> staticModels() {
  
  auto modelWeapon     = loadModel(configModelName("weapon"),Weapon::weaponDepth,true);
  auto modelPowerUp    = loadModel(configModelName("powerup"),PowerUp::powerUpDepth,true);
  auto modelProjectile = loadModel(configModelName("ice"),Projectile::projDepth,true);
  auto modelWall       = loadModel(configModelName("wall"),10);
  auto modelPlayer     = loadModel(configModelName("goodguy"),Player::playerDepth,true);
  //auto modelTriton     = loadModel(configModelName("triton"),25,true);
  auto modelDragon1     = loadModel(configModelName("dragon1"), 100,true);
  auto modelDragon2     = loadModel(configModelName("dragon2"), 100,true);
 
  auto modelTorch      = loadModel(configModelName("torch"),1,true);
  auto modelFakeTorch   = loadModel(configModelName("torch"),1,true);
 
  auto modelcoloumn1   = loadModel(configModelName("coloumn1"),40,true);
  auto modelcoloumn2   = loadModel(configModelName("coloumn2"),20,true);
  auto modelcoloumn3   = loadModel(configModelName("coloumn3"),20,true);

  auto cubes = gx::loadCube();
  auto skybox = gx::loadSkybox();
  auto ground = gx::loadGround(2000.0f, 0.0f, "models/floor.jpg");
  std::vector<gx::graphicsEntity> entitiesData;

  generalOffset[WALL] = entitiesData.size();
  entitiesData.push_back(std::move(modelWall));  //wall

  generalOffset[WEAPON] = entitiesData.size();
  entitiesData.push_back(std::move(modelWeapon)); //weapon

  generalOffset[POWER_UP] = entitiesData.size();
  entitiesData.push_back(std::move(modelPowerUp)); //powerup

  generalOffset[GROUND] = entitiesData.size();
  entitiesData.push_back(std::move(ground));  //ground

  //  generalOffset[TRITON] = entitiesData.size();
    //entitiesData.push_back(std::move(modelTriton));
  generalOffset[TORCH] = entitiesData.size();
  entitiesData.push_back(std::move(modelTorch));

  generalOffset[FAKETORCH] = entitiesData.size();
  entitiesData.push_back(std::move(modelFakeTorch));


  generalOffset[DRAGON1] = entitiesData.size();
  entitiesData.push_back(std::move(modelDragon1));

  generalOffset[DRAGON2] = entitiesData.size();
  entitiesData.push_back(std::move(modelDragon2));

  generalOffset[COLOUMN1] = entitiesData.size();
  entitiesData.push_back(std::move(modelcoloumn1));

  generalOffset[COLOUMN2] = entitiesData.size();
  entitiesData.push_back(std::move(modelcoloumn2));

  generalOffset[COLOUMN3] = entitiesData.size();
  entitiesData.push_back(std::move(modelcoloumn3));

  auto projs = initProjectileModels(entitiesData.size());
  entitiesData.insert(entitiesData.end(), std::make_move_iterator(projs.begin()),
                                          std::make_move_iterator(projs.end())); //projectile
  auto players = initPlayerModels(entitiesData.size());
  entitiesData.insert(entitiesData.end(), std::make_move_iterator(players.begin()),
                                          std::make_move_iterator(players.end())); //player

  entitiesData.insert(entitiesData.end(),std::make_move_iterator(cubes.begin()),
                                         std::make_move_iterator(cubes.end())); 
  return entitiesData;
}

std::vector<gx::graphicsEntity> dynamicModels() {
  // MODEL LOADING
  auto modelPlayer   = loadModel(configModelName("animated"),Player::playerDepth,true);
  auto modelMinotaur = loadModel(configModelName("animated"),Player::playerDepth,true);
  if(modelPlayer.rootBone.animations.empty()) {
    std::cout << "Error, model not animated" << std::endl;
  }
  if(modelMinotaur.rootBone.animations.empty()) {
    std::cout << "Error, model not animated" << std::endl;
  }
    //setup drawing data
  std::vector<gx::graphicsEntity> entitiesData;
  entitiesData.push_back(std::move(modelPlayer));
  entitiesData.push_back(std::move(modelMinotaur));
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
    userInput(), animCalc(),
    lights(gx::vector4f(1,1,1),0.1,0.1,0.0f),
    display(),
    entities(display,staticModels(),uniforms()),
    animatedDrawer(display,dynamicModels(),uniforms()),
    skyboxDrawer(display.storage()),
    particles(display,particlesData(),std::vector<uniform::block*>(1,&(display.storage()))),
    Hud(),Lobby(), Score(ConfigManager::numPlayers()),
    playerDirection(0.0, 1.0f,0.0),//will be changed by init packet
    playerStartDirection(0.0, 1.0f,0.0),
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
    //sf::Clock profilerTime;
    //float drawCalls;
    //float hudCalls; 
    //profilerTime.restart();

  // clear the buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  gx::debugout << "glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT";
  gx::debugout << "| GL_STENCIL_BUFFER_BIT);" << gx::endl;

  // draw...
  this->entities.draw();
  this->animatedDrawer.draw();
  this->skyboxDrawer.draw();
  
  //drawCalls = profilerTime.getElapsedTime().asMilliseconds();
  //profilerTime.restart();

  glDepthMask(GL_FALSE);
  this->particles.draw();
 	glDepthMask(GL_TRUE);
 
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
  //  hudCalls = profilerTime.getElapsedTime().asMilliseconds();
  //profilerTime.restart();

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

  //std::cout << "draw Call " << drawCalls << " HudCall " << hudCalls << std::endl;
}

void gx::graphicsClient::updatePosition(vector4f pos) {
  this->playerPosition = pos;
  this->setCamera();
}

void gx::graphicsClient::updateDirection(vector3f dir) {
  this->playerDirection = dir;
  this->setCamera();
}

void gx::graphicsClient::clearEntities() {
  lights.clear();

  this->entities.reset();
  this->animatedDrawer.reset();
  this->particles.reset();
}

void gx::graphicsClient::addEntity(Entity* ent) {
  const auto& entity = *ent;
  const auto& type = entity.getType();

  staticDrawer::instanceData inst;
  inst.scale = 1;
  inst. pos = vector4f(0,0,0) + entity.getPosition();
  inst.dirY = entity.getDirection();
  inst.type = generalOffset[entity.getType()];
  this->entities.addInstance(inst);
}

void gx::graphicsClient::addEntity(Player* ent) {
  const auto& entity = *ent;
  dynamicDrawer::instanceData inst;
  inst.scale = 1;
  inst.pos   = vector4f(0,0,0) + entity.getPosition();
  inst.dirY  = entity.getDirection();
  //inst.type  = playerBase + (entity.isMinotaur() ? 1 : 0);
  inst.type = (entity.isMinotaur() ? 1 : 0);
  auto anim  = this->animCalc.updatePlayer(entity);
  inst.animation = anim.first;
  inst.timePosition = anim.second;
  this->animatedDrawer.addInstance(inst);
}

void gx::graphicsClient::addEntity(Projectile* ent) {
  const auto& entity = *ent;
  lights.addLight(vector4f(0,0,0) + entity.getPosition());
  if(entity.getMagicType() == FIR1 || entity.getMagicType() == FIR2 || 
     entity.getMagicType() == FIR3) {
	  particleDrawer::instanceData inst;
    inst.position = vector4f(0,0,0) + entity.getPosition();
    inst.type = 0;
    this->particles.addInstance(inst);
    if(entity.getMagicType() == FIR2 || entity.getMagicType() == FIR3) this->particles.addInstance(inst); //draw more for bigger fire
    if(entity.getMagicType() == FIR3) this->particles.addInstance(inst); //draw more for bigger fire
  } else {
    staticDrawer::instanceData inst;
    inst.scale = ProjInfo[entity.getMagicType()].size;
    inst.pos   = vector4f(0,0,0) + entity.getPosition();
    inst.dirY  = entity.getDirection();
    inst.type  = projectileBase + projectileOffset[entity.getMagicType()];
    this->entities.addInstance(inst);
  }
}

void gx::graphicsClient::updateHUD(int id, const std::vector<Player>& players) {
  this->Hud.updateDir(this->playerDirection);
  this->Hud.updateHUD(id, players);
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
  lights.clearStatic();

  this->entities.resetStatic();
  this->animatedDrawer.resetStatic();
  this->particles.resetStatic();


  for(auto itr = e.begin(); itr != e.end(); itr++) {
    const auto& e1 = *itr;
    staticDrawer::instanceData inst;
    inst.scale = 1;
    inst.pos = vector4f(0,0,0) + e1->getPosition();
    inst.dirY = e1->getDirection();
    inst.type = generalOffset[e1->static_entity_type];
    inst.scale = static_cast<GLfloat>(e1->scale);
    this->entities.addStaticInstance(inst);

    if(e1->static_entity_type == TORCH || e1->static_entity_type == FAKETORCH) {
      if(e1->static_entity_type == TORCH) {
         lights.addStaticLight(vector4f(0,0,0) + e1->getPosition());
      }
      particleDrawer::instanceData inst;
      inst.position = vector4f(0,0,.5) + e1->getPosition();
      inst.type = 0;
      this->particles.addStaticInstance(inst);
    } 
  
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
