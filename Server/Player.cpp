#include "Player.h"

#include "PowerUp.h"
#include "Projectile.h"
#include "WeaponFist.h"
#include "WeaponFire.h"

const float Player::playerWidth = 1.0f;
const float Player::playerHeight = 1.0f;
const float Player::playerDepth = 3.0f;

//these have to be functions because calling configManager stuff to initialize
//globals is undefined behavior
length_t Player::MOVESCALE() {
  return ConfigManager::playerMovescale();
}
length_t Player::AIRMOVESCALE() {
  return ConfigManager::playerAirMovescale();
}
length_t Player::JUMPSPEED() {
  return ConfigManager::playerJumpSpeed();
}
int Player::MAXJUMP() {
  return ConfigManager::playerMaxJump();
}

Player::Player(){}// this->init(0,0,0,0,NULL);}
Player::~Player(void){}
Player::Player(v3_t pos, int assigned_id, Map * m)
{
 this->init(pos, assigned_id, m);
}

void Player::init(v3_t pos, int assigned_id, Map * m)
{
  //std::cout<<"a player is created"<<std::endl;
  velocity = v3_t(0,0,0);
  acceleration = v3_t(0,0,0);
  oldJumpVelocity = v3_t(0,0,0);
  minotaur = false;
	dead = false;
  canJump = true;
  jumpCount = 0;
	player_id = assigned_id;
	position = pos;
  direction = v3_t(0,0,0);
  defense = ConfigManager::playerDef();
  health = ConfigManager::playerHp();
  healthRegen = ConfigManager::playerHpRegen();
	maxHealth = ConfigManager::playerMaxHp();
	speed = 1;
  attackSpeed = 1;
  mana = ConfigManager::playerMp();
  manaRegen = ConfigManager::playerMpRegen();
	maxMana = ConfigManager::playerMaxMp();
	castDownCounter = sf::Clock();
  speedUpCounter = sf::Clock();
  speedUp = false;
	map = m;
	weapon[0] = new WeaponFist(position, this->map);
	weapon[1] = new WeaponFire(position, this->map); //TODO add this to entities if we want it to drop
	current_weapon_selection = 1;
  chargedProjectile = nullptr;
  generateBounds(position);
  m->addToQtree(this);
}

void Player::setAsMinotaur(bool b)
{
  minotaur = b;
  if(b)
  {
    defense = ConfigManager::minotaurDef();
    health = ConfigManager::minotaurHp();
    healthRegen = ConfigManager::minotaurHpRegen();
	  maxHealth = ConfigManager::minotaurMaxHp();
    mana = ConfigManager::minotaurMp();
    manaRegen = ConfigManager::minotaurMpRegen();
	  maxMana = ConfigManager::minotaurMaxMp();
  } else {
    defense = ConfigManager::playerDef();
    health = ConfigManager::playerHp();
    healthRegen = ConfigManager::playerHpRegen();
	  maxHealth = ConfigManager::playerMaxHp();
    mana = ConfigManager::playerMp();
    manaRegen = ConfigManager::playerMpRegen();
	  maxMana = ConfigManager::playerMaxMp();
  }
}

bool Player::isMinotaur()
{
  return minotaur;
}

void Player::generateBounds(v3_t pos){
  //init the bounding objects 
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(pos.x,pos.y,pos.z),
    BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
    playerWidth,playerHeight,playerDepth);
  b->setEntity(this);
  boundingObjs.push_back(b);
}

bool Player::attackBy(DeadlyEntity *other)
{
	if(other)
	{
		return damageBy(other);
	}
	else
		return false;
}

bool Player::damageBy(DeadlyEntity *deadly)
{
	float damage = deadly->getStrength() - defense;
	damage = ( damage > 0? damage: 0);
	float newHealth = (health - damage);
	health = (newHealth > 0 ? newHealth : 0);
  dead = health==0;
  if(dead)
    die();
	return true;
}

void Player::handleAction(ClientGameTimeAction a) {
  //std::cout<<"PLAYER: "<<player_id<<" handling packet from "<< a.player_id<<std::endl;
	if(a.player_id == player_id)
		handleSelfAction(a);
}

bool Player::moveTowardDirection(move_t inputDir, bool jump)
{
  v3_t movementDirection;
  //get movement direction
  if(inputDir != NULL_DIR) {
    // x' = xcos@ - ysin@
    // y' = xsin@ + ycos@ 
    length_t xp = direction.x * cos(movementAngles[inputDir]) - direction.y * sin(movementAngles[inputDir]);
    length_t yp = direction.x * sin(movementAngles[inputDir]) + direction.y * cos(movementAngles[inputDir]);
    movementDirection = v3_t(xp,yp,0);
    movementDirection.normalize();
  }

  //if jump add jump velocity, 
  // and not free fall with no jumps
  if(jump && canJump && 
    !(jumpCount == 0 && velocity.z < getGravity().z * ConfigManager::serverTickLengthSec() * 5)){
    //add jump velocity
    v3_t jumpDir = movementDirection;
    jumpDir.z = 1;
    jumpDir.scale(JUMPSPEED());
    velocity = velocity - oldJumpVelocity;
    velocity += jumpDir;
    velocity.z = jumpDir.z; //reset z velocity (for double jumping)
    
    //remember old velocity
    jumpDir.z = 0;
    oldJumpVelocity = jumpDir;

    if(++jumpCount >= MAXJUMP())
      canJump = false;
  }
  
  //adjust movement
  if(jumpCount > 0) //move less if you are in the air
    movementDirection.scale(speed * AIRMOVESCALE());
  else
    movementDirection.scale(speed * MOVESCALE());

  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectMovement ){
      movementDirection.scale(BuffInfo[buff->first].movementMultiplier);
    }
  }

  movementDirection = correctMovement(movementDirection, true);
  position += movementDirection;
  updateBounds();
	return true;
}

bool Player::correctMovementHit( Entity* e ){
  Entity_Type etype = e->getType();
  return etype == PLAYER || etype == WALL;
}

void Player::die()
{
  map->removeFromQtree(this);
  render = false;
}

void Player::respawn(v3_t pos)
{
  this->init(pos, player_id, map);
  render = true;
}


void Player::update(){
  // No need to update when user is dead
  if(dead)
    return;
  //powerup shit
  //std::cout << "buff size " << buffs.size() << std::endl;
  for(auto buff = buffs.begin(); buff != buffs.end();){
    buff->second--;
    if( buff->second <= 0 ){
      buff = buffs.erase(buff);
    } else {
      buff++;
    }
  }

  if(speedUp && speedUpCounter.getElapsedTime().asMilliseconds() > speedUpTime) {
     speedUp = false;
     attackSpeed = 1.0;
  }

  //
  if(chargedProjectile ) {
    chargedProjectile->setPosition(getProjectilePosition());
  }

  //pick up weapon stuff
  pickup = nullptr;
  pickupWeaponType = UNK;

  //update movement
  acceleration = getGravity();
  velocity += acceleration * ConfigManager::serverTickLengthSec();
  //v3_t attemptMove = velocity * ConfigManager::serverTickLengthSec(); TODO use this
  //position += correctMovement( attemptMove, false );
  position += velocity * ConfigManager::serverTickLengthSec();
  
  //I disabled health regen and mana regen  (BOWEN)
  health+=healthRegen;
  health = (health > maxHealth? maxHealth : health);
  mana+=manaRegen;
  mana = (mana > maxMana? maxMana : mana);
  updateBounds();
}

void Player::restartJump(length_t zPosFix){
  velocity = velocity - oldJumpVelocity;
  oldJumpVelocity = ZEROVEC;

  if(zPosFix > 0){
    velocity.z = 0; //zPosFix/ConfigManager::serverTickLengthSec();
    canJump = true;
    jumpCount = 0;
  }
}

void Player::handleSelfAction(ClientGameTimeAction a) {
	// User is still casting their spell (in case we have spell cast time)
	// This is NOT spell cool down time.
 	if(castDownCounter.getElapsedTime().asMilliseconds() < castDownTime )
		return;

	//start of movement logic
	moveTowardDirection(a.movement, a.jump);
  direction = v3_t(a.facingDirection.x, a.facingDirection.y, a.facingDirection.z);
	updateBounds();

  //try pick up
  if(a.pickup && pickup ){
    weapon[current_weapon_selection]->dropDown(position);
    weapon[current_weapon_selection] = pickup;
    pickup->pickUp();
  }

	//start of attacking logic
	if(a.attackRange || a.attackMelee) {
		attack(a);
	} else { //TODO @alvin, this porbably causes the trail
    if(chargedProjectile) {
      v3_t v = direction;
      v.normalize();
      v.scale(ProjInfo[chargedProjectile->getMagicType()].speed); //TODO no magic numbers @alvin, this should be determined by magic type
      chargedProjectile->fire(v);
      chargedProjectile = nullptr;
    }
  }

  if(a.switchWeapon) {
    ++current_weapon_selection;
    current_weapon_selection = current_weapon_selection % MAXWEAPONS;
    std::cout << "switch to " << WeaponNames[weapon[current_weapon_selection]->getWeaponType()] << std::endl;
  }
}

void Player::handleOtherAction( ClientGameTimeAction) {
	//since we are modeling projectiles, we are just gonna check for melee
}

v3_t Player::getProjectilePosition() {
 
  v3_t temp = position;

  v3_t d = direction;
  d.normalize();
  d.scale(1.5);
  temp += d;
 
  return temp;
}

// this do substraction of stemina, respond to the user to render the attak animation  
void Player::attack( ClientGameTimeAction a) {
	Weapon* currentWeapon = weapon[current_weapon_selection];

	if(a.attackRange){
    if( !currentWeapon->canUseWeapon(true, this) || currentWeapon->getMpCost() > mana){
		  return;
	  }
	  mana -= currentWeapon->getMpCost();
	  chargedProjectile = currentWeapon->attackRange(direction, getProjectilePosition(), this);
	}
	else if(a.attackMelee){
		if( !currentWeapon->canUseWeapon(false, this)){
			return;
		}
		currentWeapon->attackMelee(direction, position, this);
	}

	attacking = true;
	return;
}


std::string Player::getString()
{
	std::stringstream returnString;
	returnString<<"ID:"<< player_id <<" x="<<position.x<< " y="<<position.y<< " z="<<position.z<<std::endl;
	returnString<< "velocity " << velocity << std::endl;
  returnString<< "Dx="<<direction.x<< " Dy="<<direction.y<< " Dz="<<direction.z<<std::endl;
	return returnString.str();
}

void Player::updateBounds(){
  //update the bounding objects
  //boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
  //BoundingObj::vec3_t direct(direction.x, direction.y,0);
  //boundingObjs[0]->rotate(direct,BoundingObj::vec3_t(0,0,1));
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Player::updateBoundsSoft(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Player::handleCollisions(){
  std::vector<std::pair<Entity*,BoundingObj::vec3_t>> entities =  detectCollision();
  bool restart = false;
  int restarts = 0;

  for( auto it = entities.begin(); it != entities.end(); ){
    Entity * e = it->first;
    switch( e->getType() ) {
      case WALL:
        //std::cout << "wall" << std::endl;
        restart = collideWall(*it);
        break;
      case PLAYER:
        ////std::cout << "player" << std::endl;
        restart = collidePlayer(*it);
        break;
      case PROJECTILE:
        //std::cout << "proj" << std::endl;
        restart = collideProjectile(*it);
        break;
      case WEAPON:
        pickup = (Weapon*)e;
        pickupWeaponType = ((Weapon*)e)->getWeaponType();
        break;
      case POWER_UP:
        std::cout << "hit powerup" << std::endl;
        collidePowerUp(*it);
        //((PowerUp *)&it)->onCollision(this);
        break;
      default:
        break;
    }

    //different position now, needs to see what it hit
    //TODO this could cause an infinite loop
    if(restart){
      restarts++;
      restart = false;
      entities = detectCollision();
      it = entities.begin();
    } else {
      it++;
    }

    if( restarts > 3 ) break;
  }
}

bool Player::collideWall(const std::pair<Entity*,BoundingObj::vec3_t>& p){
  BoundingObj::vec3_t fixShit = p.second;
  if(fixShit.z == 0)
    std::cout << "fixit " << fixShit << std::endl;
  restartJump(fixShit.z);
  position += p.second;
  updateBounds();
  return true;
}

bool Player::collidePlayer(const std::pair<Entity*,BoundingObj::vec3_t>& p){
  BoundingObj::vec3_t fixVec = p.second;
  restartJump(fixVec.z);
  position += fixVec;
  updateBounds();
  return true;
}

bool Player::collideProjectile(const std::pair<Entity*,BoundingObj::vec3_t>& p){
  if(((Projectile *)p.first)->getOwner() != this) {
    std::cout << "OW hit "<< player_id << std::endl;
    attackBy((Projectile *)p.first);
    std::cout << health  << " HP left" << " for " << player_id << std::endl;
  }
  return false;
}

bool Player::collidePowerUp(const std::pair<Entity*,BoundingObj::vec3_t>& p){
  BUFF ptype = ((PowerUp*)p.first)->getBuffType();
  
  auto buff = buffs.begin();
  for(; buff != buffs.end(); buff++){
    if( buff->first == ptype){ //found one that is the same, reset timer
      buff->second = BuffInfo[ptype].ticksEffect;
      break;
    }
  }

  if(buff == buffs.end()){//didn't find same type
    buffs.push_front(std::pair<BUFF,int>(ptype,BuffInfo[ptype].ticksEffect));
  }

  p.first->removeFromMap();
  return false;
}

void Player::setHealth(float h) {
	health = h;
//  std::cout<<"health set to "<<health<<std::endl;
}

void Player::setSpeed(float s) {
	speed = s;
}

void Player::setMana(float m) {
	mana = m;
}

void Player::serialize(sf::Packet& packet) const {
    Entity::serialize(packet);
    packet << this->player_id;
    //acceleration.serialize(packet);
    //velocity.serialize(packet);
    //oldJumpVelocity.serialize(packet);
    packet << dead; 
    packet << minotaur; //might be private
    packet << name;
    packet << health;
    packet << maxHealth;
    packet << mana;
    packet << maxMana;
    packet << defense;
    packet << speed;
    packet << castDownTime; //not needed on client ?
    //sf::Clock castDownCounter;
    packet << jumpCount; // not needed on client ?
    packet << canJump; //not needed on client ?
    packet << attacking;  //not neede on client ?
    //Weapon* weapon[MAXWEAPONS]; 
    // change the array to vector ?
    packet << static_cast<sf::Uint32>(pickupWeaponType);
    packet << current_weapon_selection; 
  }

  void Player::deserialize(sf::Packet& packet) {
    Entity::deserialize(packet);
    packet >> this->player_id;
    //acceleration.deserialize(packet);
    //velocity.deserialize(packet);
    //oldJumpVelocity.deserialize(packet);
    packet >>dead; 
    packet >>minotaur; //might be private
    packet >> name;
    packet >> health;
    packet >> maxHealth;
    packet >> mana;
    packet >> maxMana;
    packet >> defense;
    packet >> speed;
    packet >> castDownTime; //not needed on client ?
    //sf::Clock castDownCounter;
    packet >> jumpCount; // not needed on client ?
    packet >> canJump; //not needed on client ?
    packet >> attacking;  //not neede on client ?
    //Weapon* weapon[MAXWEAPONS]; 
    // change the array to vector ?
    sf::Uint32 pickupWeaponTypeUint32;
    packet >> pickupWeaponTypeUint32;
    pickupWeaponType = static_cast<WeaponType>(pickupWeaponTypeUint32);
    packet >> current_weapon_selection; 
  }
