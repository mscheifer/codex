#include "Player.h"

const float Player::sphereRadius = 5.0f;
const length_t Player::MOVESCALE = ConfigManager::playerMovescale();
const length_t Player::AIRMOVESCALE = ConfigManager::playerAirMovescale();
const length_t Player::JUMPSPEED = ConfigManager::playerJumpSpeed();
const int Player::MAXJUMP = ConfigManager::playerMaxJump();

Player::Player(){}// this->init(0,0,0,0,NULL);}
Player::~Player(void){}
Player::Player(v3_t pos, int assigned_id, Map * m)
{
 this->init(pos, assigned_id, m);
}

void Player::init(v3_t pos, int assigned_id, Map * m)
{
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
	defense = 5;
	health = 100;
	maxHealth = 100;
	speed = 1;
	mana = 100;
	maxMana = 100;
	castDownCounter = sf::Clock();
	map = m;
	weapon[0] = new WeaponFist(position, this->map);
	weapon[1] = new WeaponFire(position, this->map); //TODO add this to entities if we want it to drop
	current_weapon_selection = 0;
  
  generateBounds(position);
  m->addToQtree(this);
}

void Player::generateBounds(v3_t pos){
  //init the bounding objects 
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(pos.x,pos.y,pos.z),
    BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
    sphereRadius,sphereRadius,sphereRadius);
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
	return true;
}

void Player::handleAction(ClientGameTimeAction a) {
  //std::cout<<"PLAYER: "<<player_id<<" handling packet from "<< a.player_id<<std::endl;
	if(a.player_id == player_id) {
		handleSelfAction(a);
	} else {
		//handleOtherAction(a);
	}
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
    jumpDir.scale(JUMPSPEED);
    velocity = velocity - oldJumpVelocity;
    velocity += jumpDir;
    velocity.z = jumpDir.z; //reset z velocity (for double jumping)
    
    //remember old velocity
    jumpDir.z = 0;
    oldJumpVelocity = jumpDir;

    if(++jumpCount >= MAXJUMP)
      canJump = false;
  }
  
  //adjust movement
  if(jumpCount > 0) //move less if you are in the air
    movementDirection.scale(speed * AIRMOVESCALE);
  else
    movementDirection.scale(speed * MOVESCALE);

  
  movementDirection = correctMovement(movementDirection, true);

  position += movementDirection;

	return true;
}

v3_t Player::correctMovement(v3_t movementDirection, bool slide){
  Ray movementRay(v4_t(position.x,position.y,position.z), movementDirection);
  std::vector<RayCollision> colls = detectCollision(&movementRay);
  bool restart = false;
  int restarts = 0;

  for(auto coll = colls.begin(); coll != colls.end(); ){
    Entity * e = coll->e;
    v3_t acceptedMove = movementRay.getDirection();

    switch(coll->e->getType()){
    case WALL:
    case PLAYER:
      {
        //scale by tfirst
        v3_t newDir = acceptedMove;
        newDir.scale(coll->tfirst);
        
        if(slide){
          //project max "radius" onto normal and add the largest
          //adjust normal axis to be in opposite direction as movement (pi/2 - -pi/2)
          if( movementRay.getDirection().dot(coll->normalAxis) > 0 ){
            coll->normalAxis.negate();
          }
          coll->normalAxis.normalize();

          length_t maxRad;
          length_t rad;
          //TODO just doing this for now
          BoundingBox * myBox = (BoundingBox*) boundingObjs[0];
          v3_t radius = myBox->getAx();
          radius.scale(myBox->getHw());
          maxRad = radius.dot(coll->normalAxis);

          radius = myBox->getAy();
          radius.scale(myBox->getHh());
          rad = radius.dot(coll->normalAxis);
          if( rad > maxRad ){
            maxRad = rad;
          }

          radius = myBox->getAz();
          radius.scale(myBox->getHd());
          rad = radius.dot(coll->normalAxis);
          if( rad > maxRad ){
            maxRad = rad;
          }

          coll->normalAxis.scale(maxRad);
          newDir += coll->normalAxis;

          //project extra onto axis parallel and add that
          v3_t excess = acceptedMove;
          excess.scale(1.0f - coll->tfirst);
          coll->parallelAxis.normalize();
          excess.scale( excess.dot(coll->parallelAxis) );
          newDir += excess;
        }

        movementRay.setDirection(newDir);
        restart = true;
        break;
      }
    default:
      break;
    }

    if(restart){
      restarts++;
      restart = false;
      colls = detectCollision(&movementRay);
      coll = colls.begin();
    } else {
      coll++;
    }

    if( restarts > 3 ) break;
  }

  return movementRay.getDirection();
}

void Player::update(){

  //pick up weapon stuff
  pickup = nullptr;
  pickupWeaponType = UNK;

  //update movement
  acceleration = getGravity();
  velocity += acceleration * ConfigManager::serverTickLengthSec();
  position += velocity * ConfigManager::serverTickLengthSec();
  health = (health+5 > maxHealth? maxHealth : health+5);
  mana = (mana+5 > maxMana? maxMana : mana+5);
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
	}
}

void Player::handleOtherAction( ClientGameTimeAction) {
	//since we are modeling projectiles, we are just gonna check for melee
}

// this do substraction of stemina, respond to the user to render the attak animation  
void Player::attack( ClientGameTimeAction a) {
	Weapon* currentWeapon = weapon[current_weapon_selection];

	if(a.attackRange){
    if( !currentWeapon->canUseWeapon(true) || currentWeapon->getMpCost() > mana){
			return;
		}
		mana -= currentWeapon->getMpCost();
		Projectile* proj = currentWeapon->attackRange(direction, position);
    proj->setOwner(this);
	}
	else if(a.attackMelee){
		if( !currentWeapon->canUseWeapon(false)){
			return;
		}
		currentWeapon->attackMelee();
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
        //std::cout << "pick me up plz" << std::endl;
        //((Weapon*) e)->pickUp();
        //TODO finish this
        break;
      case POWER_UP:
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
  restartJump(fixShit.z);
  position += p.second;
  updateBounds();
  return true;
}

bool Player::collidePlayer(const std::pair<Entity*,BoundingObj::vec3_t>& p){
  BoundingObj::vec3_t fixVec = p.second;
  fixVec.scale(0.5f);
  position += fixVec;
  fixVec.negate();
  p.first->setPosition( p.first->getPosition() + fixVec );
  p.first->updateBounds();
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

void Player::setHealth(float h) {
	health = h;
}

void Player::setSpeed(float s) {
	speed = s;
}

void Player::setMana(float m) {
	mana = m;
}

