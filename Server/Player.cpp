#include "Player.h"

Player::Player(void){}
Player::Player(Position x, Position y, Position z, int assigned_id)
{
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
  // CANNOT JUMP IF YOU ARE NOT ON THE GROUND
  if(getTerrainHeight(position.x, position.y) != position.z)
    return;
  position = ThreeDMovement(position, direction, 10000); // TODO: Not sure if this will work
}

void Player::handleAction(ClientGameTimeAction a) {
	if(a.player_id == player_id) {
		handleSelfAction(a);
	}

}

bool Player::moveTowardDirection(User_Movement degree)
{
  if(degree == NONE)
    return false;
  // x' = xcos@ - ysin@
  // y' = xsin@ + ycos@ 
  DirectionValue newX = (DirectionValue)(direction.x * cos(degree*PI/180) - direction.y * sin(degree*PI/180));
  DirectionValue newY = (DirectionValue)(direction.x * sin(degree*PI/180) + direction.y * cos(degree*PI/180));
  double length = sqrt(newX *newX + newY * newY);
  position.velocityX = newX/length * MOVESCALE * speed;
  position.velocityY = newY/length * MOVESCALE * speed;
  position = ThreeDMovement(position, Direction(newX, newY, direction.z), GRAVITY);
  fixPosition();
  return true;
}

void Player::handleSelfAction(ClientGameTimeAction a) {

	//start of movement logic
	direction = a.facingDirection;
  if(moveTowardDirection(a.movement))
    throw "Oh... something got fucked up in player handleSelfAction";

	if(a.jump) {
		jump();
	}


	//end of calculating movement

	//start of attacking logic
	//if( a.weapon_switch)

	if(a.attack) {
		attack(a);
	}
}

void Player::handleOtherAction( ClientGameTimeAction a) {
	//check if the user can really attack


}
// this do substraction of stemina, respond to the user to render the attak animation  
void Player::attack( ClientGameTimeAction a) {

	if( weapon[current_weapon_selection].useWeapon() ) {
		// tell client to render stuff
	} else {
		// don't do anything
	}

}


std::string Player::getString()
{
  std::stringstream returnString;
  returnString<< " x="<<position.x<< " y="<<position.y<< " z="<<position.z<<std::endl;
  returnString<< " x="<<position.velocityX<< " y="<<position.velocityY<< " z="<<position.velocityZ<<std::endl;
  return returnString.str();
}
