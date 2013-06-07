#include "Player.h"

#include "PowerUp.h"
#include "Projectile.h"
#include "WeaponFist.h"
#include "WeaponFire.h"
#include "Wall.h"

const float Player::playerWidth = 3.0f;
const float Player::playerHeight = 3.0f;
const float Player::playerDepth = 7.0f;

//these have to be functions because calling configManager stuff to initialize
//globals is undefined behavior
length_t Player::MOVESCALE(bool mino) {
  if(mino)
    return ConfigManager::minotaurMovescale();
  else
    return ConfigManager::playerMovescale();
}
length_t Player::AIRMOVESCALE(bool mino) {
  if(mino)
    return ConfigManager::minotaurAirMovescale();
  else
    return ConfigManager::playerAirMovescale();
}
length_t Player::JUMPSPEED(bool mino) {
  if(mino)
    return ConfigManager::minotaurJumpSpeed();
  else
    return ConfigManager::playerJumpSpeed();
}
int Player::MAXJUMP(bool mino) {
  if(mino)
    return ConfigManager::minotaurMaxJump();
  else
    return ConfigManager::playerMaxJump();
}

Player::Player(){
}// this->init(0,0,0,0,NULL);}

Player::Player(v3_t pos, int assigned_id, Map * m): kills(0), wins(0)
{
 generateBounds(position);
 this->init(pos, assigned_id, m); 
}

void Player::reset(v3_t pos)
{
  map->removeFromQtree(this);

  this->init(pos, player_id, map);
}

void Player::init(v3_t pos, int assigned_id, Map * m)
{
  //std::cout<<"a player is created"<<std::endl;
  velocity = v3_t(0,0,0);
  acceleration = v3_t(0,0,0);
  oldJumpVelocity = v3_t(0,0,0);
  attackedDir = v3_t(0,0,0);
  minotaur = false;
	dead = false;
  canJump = true;
  shotProjectile = false;
  meleeAttack = false;
  attacked = false;
  jumpCount = 0;
	player_id = assigned_id;
	position = pos;
  direction = v3_t(0,1,0);
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
  charging = false;
  walking = false;
  collectPowerUp = false;
  ptype = NONE;
  elapsedChargeTime = 0;
  totalChargeTime = -1;
	map = m;
	weapon[0] = new WeaponFire(position, this->map, B1);
    //new WeaponFist(position, this->map); //has no bounds so it doesnt drop
  //weapon[1] = new WeaponFist(position, this->map);
	weapon[1] = new WeaponFire(position, this->map, FIR1); //TODO make this basic
	m->addEntity(weapon[1]);
  weapon[1]->pickUp();
  weapon[1]->setRespawnTime(60000);
  m->addEntity(weapon[0]);
  weapon[0]->pickUp(); //remove this
  weapon[0]->setRespawnTime(60000);

  buffs.clear();
  inactiveBuffs.clear();
  current_weapon_selection = 1;
  chargedProjectile = nullptr;

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

bool Player::isMinotaur() const
{
  return minotaur;
}

void Player::generateBounds(v3_t pos){
  //init the bounding objects 
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(pos.x,pos.y,pos.z),
    BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
    playerWidth/2.f,playerHeight/2.f,playerDepth/2.f);
  b->setEntity(this);
  boundingObjs.push_back(b);
}

void Player::removeChargingProj(){
  chargedProjectile = nullptr;
  charging = false;
}

bool Player::attackBy(Projectile *other)
{
	if(other && other->getOwner() != this)
	{
		return damageBy(other);
	}
	else
		return false;
}

bool Player::damageBy(Projectile *deadly)
{
	if (health==0) return true;

  attacked = true;
  attackedDir = deadly->getDirection();
  float damage = deadly->getStrength() - defense*getDefenseMultiplier();
  attackedMagicType = deadly->getMagicType();
	damage = ( damage > 0? damage: 0);
	float newHealth = (health - damage);
	health = (newHealth > 0 ? newHealth : 0);
  dead = health==0;

  //TODO remove this to allow charging while hit
  if(charging && !isMinotaur()) {
    chargedProjectile->live = false;
  }

  if(dead) {
    die();
    //This is a hack
    map->kills[deadly->getOwner()->player_id]++;
  }

  std::vector<BUFF> debuffs = ProjInfo[deadly->getMagicType()].debuff;
  for( auto d = debuffs.begin(); d != debuffs.end(); d++)
    applyBuff(*d);

 // std::cout << health  << " HP left" << " for " << player_id << std::endl;

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
  // not stunned
  // and not free fall with no jumps
  if(jump && canJump && 
    getMovementMultiplier() != 0 &&
    !(jumpCount == 0 && velocity.z < getGravity().z * ConfigManager::serverTickLengthSec() * 5)){
    //add jump velocity
    v3_t jumpDir = movementDirection;
    jumpDir.z = 0;
    jumpDir.scale(JUMPSPEED(isMinotaur())/4.f);
    jumpDir.z = JUMPSPEED(isMinotaur());
    velocity = velocity - oldJumpVelocity;
    velocity += jumpDir;
    velocity.z = jumpDir.z; //reset z velocity (for double jumping)
    
    //remember old velocity
    jumpDir.z = 0;
    oldJumpVelocity = jumpDir;

    if(++jumpCount >= MAXJUMP(isMinotaur()))
      canJump = false;
  }
  
  //adjust movement
  if(jumpCount > 0) //move less if you are in the air
    movementDirection.scale(speed * AIRMOVESCALE(isMinotaur()));
  else
    movementDirection.scale(speed * MOVESCALE(isMinotaur()));

  movementDirection.scale(getMovementMultiplier());
  movementDirection = correctMovement(movementDirection, true, getFeetOrigin());

  //walking if not moving, jumps are 0, or free fall
  if((std::abs(movementDirection.magnitude()) > 1.0E-8  && jumpCount == 0)){
  //  || velocity.z >= getGravity().z * ConfigManager::serverTickLengthSec() * 5){
    walking = true;
  } else {
    walking = false;
  }

  position += movementDirection;
  updateBounds();
	return true;
}

bool Player::correctMovementHit( Entity* e ){
  Entity_Type etype = e->getType();
  return etype == PLAYER || etype == WALL;
}

void Player::clearEvents(){
  upgraded = false;
  walking = false;
  shotProjectile = false;
  attacked = false;
  meleeAttack = false;
  weaponCall = false;
  collectPowerUp = false;
  attackedDir = v3_t(0,0,0);
}

void Player::die()
{
  map->removeFromQtree(this);
  render = false;
  dead = true;
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

  updateBuffs();

  //pick up weapon stuff
  pickup = nullptr;
  pickupWeaponType = NONEWEAPON;

  //update movement
  acceleration = getGravity();
  velocity += acceleration * ConfigManager::serverTickLengthSec();
  v3_t attemptMove = velocity * ConfigManager::serverTickLengthSec();
  position += correctMovement( attemptMove, false, getFeetOrigin() );
  //position += velocity * ConfigManager::serverTickLengthSec();

  if( chargedProjectile ) {
    //this is for HUD
    elapsedChargeTime = chargedProjectile->getElapsedTime();
    totalChargeTime = ProjInfo[chargedProjectile->getMagicType()].chargeTime * getChargeCD();
    if(totalChargeTime < 0)
      totalChargeTime = -1;
    chargeMagicType = chargedProjectile->getMagicType();
   
    chargedProjectile->setDirection(direction);
    chargedProjectile->setPosition(getProjectileChargePosition());
  } else {
    elapsedChargeTime = totalChargeTime = -1;
  }

  health+= healthRegen*getHealthRegenMultiplier();
  health = (health > maxHealth ? maxHealth : health);
  mana+=manaRegen*getManaRegenMultiplier();
  mana = (mana > maxMana ? maxMana : mana);
  mana = (mana < 0 ? 0 : mana);
  if(health <= 0)
    die();
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
  if(dead)
    return;

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
    weapon[current_weapon_selection]->tossAway(position, direction);
    weapon[current_weapon_selection] = pickup;
    pickup->pickUp();
    weaponCall = true;
    weaponCallType = weapon[current_weapon_selection]->getWeaponType();
  }

	//start of attacking logic
  //std::cout << " attackRng " << a.attackRange << " chrg " << (chargedProjectile == nullptr) << std::endl;
  if ( chargedProjectile && !a.attackRange ) { //@fire the projectile!
    elapsedChargeTime = totalChargeTime = -1;
    fireProjectile();
  }

  if( a.attackRange || a.attackMelee ) {
		attack(a);
  }

  if(a.switchWeapon) {
    ++current_weapon_selection;
    current_weapon_selection = current_weapon_selection % MAXWEAPONS;
    std::cout << "switch to " << WeaponNames[weapon[current_weapon_selection]->getWeaponType()] << std::endl;
    weaponCall = true;
    weaponCallType = weapon[current_weapon_selection]->getWeaponType();
  }
}

void Player::fireProjectile() {
  chargedProjectile->setPosition(getProjectilePosition());
  v3_t v = direction;
  v.normalize();
  if(minotaur) {
    chargedProjectile->fireMutiple(v,getStrengthMultiplier(),5);
  } else {
    chargedProjectile->fire(v,getStrengthMultiplier());
  }
    
  chargedProjectile = nullptr;
    
  charging = false;
  shotProjectile = true;
}
void Player::handleOtherAction( ClientGameTimeAction) {
	//since we are modeling projectiles, we are just gonna check for melee
}

v3_t Player::getProjectilePosition() {
  v3_t temp = position;
  v3_t d = direction;
  d.normalize();
  float size;
  if(chargedProjectile)
    size = ProjInfo[chargedProjectile->getMagicType()].size;
  else
    size = ProjInfo[weapon[current_weapon_selection]->getBasicAttack()].size;
  d.scale(size + 0.5f + playerHeight/2.f);
  temp += d;
  return temp;
}

v3_t Player::getProjectileChargePosition() {
  v3_t temp = position;
  v3_t d(0,0,1);
  float size;
  if(chargedProjectile)
    size = ProjInfo[chargedProjectile->getMagicType()].size;
  else
    size = ProjInfo[weapon[current_weapon_selection]->getBasicAttack()].size;
  d.scale(size + 0.5f + playerDepth/2.f);
  temp += d;
  return temp;
}

// this do substraction of stemina, respond to the user to render the attak animation  
void Player::attack( ClientGameTimeAction a) {
  if(chargedProjectile)
    return;
	Weapon* currentWeapon = weapon[current_weapon_selection];
  if(a.attackRange){
    if( !currentWeapon->canUseWeapon(true, this) || currentWeapon->getMpCost() > mana){
      charging = false;
		  return;
	  }
	  mana -= currentWeapon->getMpCost();
	  chargedProjectile = currentWeapon->attackRange(direction, getProjectileChargePosition(), this);
    charging = true;
	}
	else if(a.attackMelee){
		if( !currentWeapon->canUseWeapon(false, this)){
		  return;
		}
    meleeAttack = true;
    v3_t dir = direction;
    dir.normalize();
    dir.scale(Projectile::meleeWidth/2.f);
	  currentWeapon->attackMelee(direction, position+dir, this);
	}

	attacking = true; //todo wtf is this
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
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
  BoundingObj::vec3_t direct(direction.x, direction.y,0);
  boundingObjs[0]->rotate(direct,BoundingObj::vec3_t(0,0,1));
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Player::updateBoundsSoft(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
}

v3_t Player::getFeetOrigin(){
  v3_t origin(0,0,-1*(playerHeight/2.1f));
  return origin + position;
}

void Player::handleCollisions(){
  std::vector<std::pair<Entity*,BoundingObj::vec3_t>> entities =  detectCollision();
  std::vector<Entity *> alreadyCollided;
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
        //std::cout << "player" << std::endl;
        restart = collidePlayer(*it);
        break;
      case WEAPON:
        pickup = (Weapon*)e;
        pickupWeaponType = ((Weapon*)e)->getWeaponType();
        break;
      case POWER_UP:
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
 // std::cout << "("<<p.second.x << "," << p.second.y << "," << p.second.z<< ")" <<std::endl;
  BoundingObj::vec3_t fixShit = p.second;
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


bool Player::collidePowerUp(const std::pair<Entity*,BoundingObj::vec3_t>& p){
  BUFF ptype = ((PowerUp*)p.first)->getBuffType();
  this->ptype = ptype;
  applyBuff(ptype);
  ((PowerUp*)p.first)->pickUp();
  collectPowerUp = true;
  return false;
}

void Player::updateBuffs(){
  for(auto buff = inactiveBuffs.begin(); buff != inactiveBuffs.end();){
    buff->second -= (int)ConfigManager::serverTickLengthMilli();
    if( buff->second <= 0 ){
      buff = inactiveBuffs.erase(buff);
    } else {
      buff++;
    }
  }

  std::list<std::pair<BUFF, int>> toAdd;
  for(auto buff = buffs.begin(); buff != buffs.end();){
    buff->second -= (int)ConfigManager::serverTickLengthMilli();
    if( buff->second <= 0 ){      
      toAdd.push_back(getBuffReplacement(buff->first));
      buff = buffs.erase(buff);
    } else {
      buff++;
    }
  }

  for(auto buff = toAdd.begin(); buff != toAdd.end(); buff++){
    if(buff->first != NONE)
      buffs.push_back(*buff);
  }

  //std::cout << "BUFFS" << std::endl;
  //for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
  //  std::cout << " buff " << buff->first << " ";
  //}
  //std::cout << std::endl << std::endl;

}

void Player::applyBuff( BUFF b){
  auto buff = buffs.begin();
  for(; buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].code == BuffInfo[b].code ){ //found one that is the same, reset timer
      //buff level > b level - add b to inactive
      if( BuffInfo[buff->first].level > BuffInfo[b].level ){  
        addInactiveBuff( b, BuffInfo[b].milliEffect );
        //buff level < b level - move buff to inactive
      } else if ( BuffInfo[buff->first].level < BuffInfo[b].level ){  
        addInactiveBuff( buff->first, buff->second );
        buffs.erase(buff);
        buffs.push_front(std::pair<BUFF,int>(b,BuffInfo[b].milliEffect));
      } else {  //code and level is same reset timer
        buff->second = BuffInfo[b].milliEffect;
      }
      return;
    }
  }

  if(buff == buffs.end()){//didn't find same code
    buffs.push_front(std::pair<BUFF,int>(b,BuffInfo[b].milliEffect));
  }
}

void Player::addInactiveBuff( BUFF b, int time ){
  auto buff = inactiveBuffs.begin();
  for(; buff != inactiveBuffs.end(); buff++){
    if( buff->first == b ){ //found one that is the same, reset timer
      buff->second = time > buff->second ? time : buff->second;
      break;
    }
  }

  if(buff == inactiveBuffs.end()){//didn't find same code
    inactiveBuffs.push_front(std::pair<BUFF,int>(b,time));
  }
}

std::pair<BUFF, int> Player::getBuffReplacement( BUFF b ){
  std::pair<BUFF, int> candidate(NONE,0);

  auto buff = inactiveBuffs.begin();
  for(; buff != inactiveBuffs.end(); buff++){
    if( BuffInfo[buff->first].code == BuffInfo[b].code ){ //found one that is the same, reset timer
      if( BuffInfo[buff->first].level >= BuffInfo[candidate.first].level ){
        candidate.first = buff->first;
        candidate.second = buff->second;
      }
    }
  }

  if(candidate.first != NONE){
    inactiveBuffs.remove(candidate);
  }

  return candidate;
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

float Player::getAttackCD() const{
  float cdMult = 1;
  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectAttackCD ){
      cdMult *= (BuffInfo[buff->first].attackCDMultiplier);
    }
  }
  return cdMult;
}

float Player::getChargeCD() const{
  float cdMult = 1;
  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectChargeCD ){
      cdMult *= (BuffInfo[buff->first].chargeCDMult);
    }
  }
  return cdMult;
}

float Player::getStrengthMultiplier() const{
  float strMult = 1;
  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectStrength ){
      strMult *= (BuffInfo[buff->first].strengthMultiplier);
    }
  }
  if(isMinotaur())
    strMult*= StringToNumber<float>(ConfigManager::configMap["minoStrMult"]);
  return strMult;
}

float Player::getMovementMultiplier() const{
  float movementMult = 1;
  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectMovement ){
      movementMult *= BuffInfo[buff->first].movementMultiplier;
    }
  }
  return movementMult;
}

float Player::getManaRegenMultiplier() const{
  float manaMultiplier = 1;
  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectManaRegen )
        manaMultiplier += BuffInfo[buff->first].manaBonus;
  }
  return manaMultiplier;
}

float Player::getHealthRegenMultiplier() const{
  float healthMultiplier = 1;
  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectHealthRegen )
        healthMultiplier += BuffInfo[buff->first].healthBonus;
  }
  //std::cout << "health Mult " << healthMultiplier << std::endl;
  return healthMultiplier;
}

float Player::getDefenseMultiplier() const{
  float defenseMultiplier = 1;
  for(auto buff = buffs.begin(); buff != buffs.end(); buff++){
    if( BuffInfo[buff->first].affectDefense )
        defenseMultiplier *= BuffInfo[buff->first].defenseMult;
  }
  return defenseMultiplier;
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
    packet << charging;
    packet << walking;
    packet << shotProjectile;
    packet << attacked;
    packet << player_id;
    packet << kills;
    packet << wins;
    packet << static_cast<sf::Uint32>(buffs.size());
    for (auto itr = buffs.begin(); itr!=buffs.end(); itr++) {
      packet << static_cast<sf::Uint32>((*itr).first);
      packet << (*itr).second;
    }
    packet << meleeAttack;
    packet << weaponCall;
    packet << static_cast<sf::Uint32>(weaponCallType);

    //held weapons
    packet << static_cast<sf::Uint32>(weapon[0]->getWeaponType());
    packet << static_cast<sf::Uint32>(weapon[1]->getWeaponType());

    //this is for charing HUD
    packet << elapsedChargeTime;
    packet << totalChargeTime;
    packet << static_cast<sf::Uint32>(chargeMagicType);
    packet << static_cast<sf::Uint32>(attackedMagicType);

    packet << collectPowerUp;
    packet << static_cast<sf::Uint32>(ptype);
    packet << upgraded;
    attackedDir.serialize(packet);
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
    packet >> charging;
    packet >> walking;
    packet >> shotProjectile;
    packet >> attacked;
    packet >> player_id;
    packet >> kills;
    packet >> wins;
    sf::Uint32 size = 0; 
    buffs.clear();
    packet >> size;
    for (; size > 0; size--){
      sf::Uint32 buff;
      int time;
      packet >> buff;
      packet >> time;
      buffs.push_back(std::pair<BUFF,int>(static_cast<BUFF>(buff),time));
    }
    
    packet >> meleeAttack;
    packet >> weaponCall;
    sf::Uint32 weaponCallType32;
    packet >> weaponCallType32;
    weaponCallType = static_cast<WeaponType>(weaponCallType32);
    
    //held weapons
    sf::Uint32 weaponType32;
    packet >> weaponType32;
    weapon1 = static_cast<WeaponType>(weaponType32);
    packet >> weaponType32;
    weapon2 = static_cast<WeaponType>(weaponType32);

    //this is for charging HUD
    packet >> elapsedChargeTime;
    packet >> totalChargeTime;
    packet >> weaponType32;
    chargeMagicType = static_cast<MAGIC_POWER>(weaponType32);
    
    packet >> weaponType32;
    attackedMagicType = static_cast<MAGIC_POWER>(weaponType32);

    packet >> collectPowerUp;
    packet >> weaponType32;
    ptype = static_cast<BUFF>(weaponType32);
    packet >> upgraded;
    attackedDir.deserialize(packet);
  }

  std::string Player::toString(){
    std::stringstream ss;
    ss << "pos " << position << std::endl;
    ss << "dir " << direction << std::endl;
    ss << this->player_id;
    //acceleration.serialize(packet);
    //velocity.serialize(packet);
    //oldJumpVelocity.serialize(packet);
    ss << dead; 
    ss << minotaur; //might be private
    ss << name;
    ss << health;
    ss << maxHealth;
    ss << mana;
    ss << maxMana;
    ss << defense;
    ss << speed;
    ss << castDownTime; //not needed on client ?
    //sf::Clock castDownCounter;
    ss << jumpCount; // not needed on client ?
    ss << canJump; //not needed on client ?
    ss << attacking;  //not neede on client ?
    //Weapon* weapon[MAXWEAPONS]; 
    // change the array to vector ?
    ss << static_cast<sf::Uint32>(pickupWeaponType);
    ss << current_weapon_selection; 
    ss << charging;
    ss << walking;
    ss << shotProjectile;
    ss << attacked;
    ss << player_id;
    ss << kills;
    ss << wins;
    ss << static_cast<sf::Uint32>(buffs.size());
    for (auto itr = buffs.begin(); itr!=buffs.end(); itr++) {
      ss << static_cast<sf::Uint32>((*itr).first);
      ss << (*itr).second;
    }
    ss << meleeAttack;
    ss << weaponCall;
    ss << static_cast<sf::Uint32>(weaponCallType);

    //held weapons
    ss << static_cast<sf::Uint32>(weapon[0]->getWeaponType());
    ss << static_cast<sf::Uint32>(weapon[1]->getWeaponType());

    //this is for charing HUD
    ss << elapsedChargeTime;
    ss << totalChargeTime;
    ss << static_cast<sf::Uint32>(chargeMagicType);

    ss << collectPowerUp;
    ss << upgraded << std::endl;
    return ss.str();
  }
