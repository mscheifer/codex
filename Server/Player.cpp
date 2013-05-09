#include "Player.h"
#include "Projectile.h"

const float Player::sphereRadius = 5.0f;

Player::Player()
{
// this->init(0,0,0,0,NULL);
}

Player::Player(Position x, Position y, Position z, int assigned_id, Map * m)
{
 this->init(x, y, z, assigned_id, m);
}

void Player::init(Position x, Position y, Position z, int assigned_id, Map * m)
{
  minotaur = false;
	dead = false;
	player_id = assigned_id;
	position.x = x;
	position.y = y;
	position.z = z;
	direction.x = 0.0;
	direction.y = -1.0;
	direction.z = 0.0;
	defense = 5;
	health = 100;
	maxHealth = 100;
	speed = 1;
	mana = 100;
	maxMana = 100;
	castDownCounter = sf::Clock();
	map = m;
	weapon[0] = new WeaponFist(position, this->map);
	weapon[1] = new WeaponFire(position, this->map);
	current_weapon_selection = 1;
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(x,y,z),BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
    sphereRadius,sphereRadius,sphereRadius);
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  boundingObjs.push_back(b);
  m->addToQtree(this);
}

Player::~Player(void)
{
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

Position getTerrainHeight(Position x, Position y)
{
	// TODO: Will need to ask map for this information
	return 0;
}
void Player::fixPosition()
{
	position.velocityX = 0;
	position.velocityY = 0;
	double terrainHeight = getTerrainHeight( position.x, position.y);
	if( position.z < terrainHeight )
	{
		position.z = terrainHeight;
		position.velocityZ = 0;
	}
}

void Player::jump()
{
	if(getTerrainHeight(position.x, position.y) == position.z)
	{
		canJump = true;
		jumpCount = 0;
	}

	if(jumpCount == MAXJUMP)
	{
		canJump = false;
		jumpCount = 0;
	} else {
		jumpCount++;
	}

	if(canJump)
	{
		position = ThreeDMovement(position, direction, 10000); // TODO: Value needs to be modified
		fixPosition();
	}
}

void Player::handleAction(ClientGameTimeAction a) {
  //std::cout<<"PLAYER: "<<player_id<<" handling packet from "<< a.player_id<<std::endl;
	if(a.player_id == player_id) {
		handleSelfAction(a);
	} else {
		//handleOtherAction(a);
	}

}

bool Player::moveTowardDirection(move_t dir)
{
	if(dir == NULL_DIR) {
		return true;
	}
	// x' = xcos@ - ysin@
	// y' = xsin@ + ycos@ 
	DirectionValue newX = (DirectionValue)(direction.x * cos(movementAngles[dir]) - direction.y * sin(movementAngles[dir]));
	DirectionValue newY = (DirectionValue)(direction.x * sin(movementAngles[dir]) + direction.y * cos(movementAngles[dir]));
	double length = sqrt(newX *newX + newY * newY);
  if(length == 0 ){
	  position.velocityX = newX * MOVESCALE * speed;
	  position.velocityY = newY * MOVESCALE * speed;
  }
  else{
	  position.velocityX = newX/length * MOVESCALE * speed;
	  position.velocityY = newY/length * MOVESCALE * speed;
  }
	position = ThreeDMovement(position, Direction(newX, newY, direction.z), GRAVITY);
	fixPosition();
	return true;
}

void Player::handleSelfAction(ClientGameTimeAction a) {
	// User is still casting their spell (in case we have spell cast time)
	// This is NOT spell cool down time.
 	if(castDownCounter.getElapsedTime().asMilliseconds() < castDownTime )
		return;
	//start of movement logic
	direction = a.facingDirection;
	moveTowardDirection(a.movement);
	updateBounds();

	if(a.jump) {
		jump();
	} else {
		// Do not allow double jumps
		//@alvin can someone tell me how does prevent double jumps? i see this logic as "if the user didn't press jump, then the user cannot jump in the future"
		canJump = false;
	}


	//end of calculating movement

	//start of attacking logic
	if(a.attackRange || a.attackMelee) {
		attack(a);
	}
}

void Player::handleOtherAction( ClientGameTimeAction) {
	//since we are modeling projectiles, we are just gonna check for melee

}
/*
void Player::onCollision(Entity* e){
	if ( e->isProjectile() ) {
		// do some calculation and deduct health. Allen you should do this part since I don't know how did you organzie the attk calcualtion
	} else if ( e->isWeapon() ){
		//pickup the weapon                                              
		weapon[1] = (Weapon*)e; // any better way to down cast?.. lol
	} // take care of player case?
}
*/

// this do substraction of stemina, respond to the user to render the attak animation  
void Player::attack( ClientGameTimeAction a) {
	Weapon* currentWeapon = weapon[current_weapon_selection];

	if(a.attackRange)
	{
    if( !currentWeapon->canUseWeapon(true) || currentWeapon->getMpCost() > mana){
			return;
		}
		mana -= currentWeapon->getMpCost();
    std::cout << "dir " << direction << " pos " << position << std::endl;
		Projectile* proj = currentWeapon->attackRange(direction, position);
    proj->setOwner(this);
	}
	else if(a.attackMelee)
	{
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
	returnString<< "Vx="<<position.velocityX<< " Vy="<<position.velocityY<< " Vz="<<position.velocityZ<<std::endl;
  returnString<< "Dx="<<direction.x<< " Dy="<<direction.y<< " Dz="<<direction.z<<std::endl;
	return returnString.str();
}

void Player::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
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
