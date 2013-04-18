#include "Player.h"

Player::Player(void){}
Player::Player(int x, int y, int z, int assigned_id)
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

int getTerrainHeight(int x, int y)
{
  return 0;
}
void Player::fixPosition()
{
  position.velocityX = 0;
  position.velocityY = 0;
  int terrainHeight = getTerrainHeight( position.x, position.y);
  if( position.z < terrainHeight )
  {
    position.z = terrainHeight;
    position.velocityZ = 0;
  }
}
void Player::moveForward()
{
  // Normalizing x and y to be 1
  float length = sqrt(direction.x *direction.x + direction.y * direction.y);
  position.velocityX = direction.x/length * MOVESCALE;
  position.velocityY = direction.y/length * MOVESCALE;
  position = ThreeDMovement(position, direction, GRAVITY); // TODO: Not sure if this will work
  fixPosition();
}

void Player::moveBackward()
{
  float length = sqrt(direction.x *direction.x + direction.y * direction.y);
  position.velocityX = -direction.x/length * MOVESCALE;
  position.velocityY = -direction.y/length * MOVESCALE;
  position = ThreeDMovement(position, direction, GRAVITY); // TODO: Not sure if this will work
  fixPosition();
}

void Player::moveRight()
{
  // x' = xcos@ - ysin@
  // y' = xsin@ + ycos@ 
  float newX = -direction.y;
  float newY = direction.x;
  float length = sqrt(newX *newX + newY * newY);
  position.velocityX = newX/length * MOVESCALE;
  position.velocityY = newY/length * MOVESCALE;
  position = ThreeDMovement(position, Direction(newX, newY, direction.z), GRAVITY); // TODO: Not sure if this will work
  fixPosition();
}
void Player::moveLeft()
{
  // x' = xcos@ - ysin@
  // y' = xsin@ + ycos@ 
  float newX = direction.y;
  float newY = -direction.x;
  float length = sqrt(newX *newX + newY * newY);
  position.velocityX = newX/length * MOVESCALE;
  position.velocityY = newY/length * MOVESCALE;
  position = ThreeDMovement(position, Direction(newX, newY, direction.z), GRAVITY); // TODO: Not sure if this will work
  fixPosition();
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

void Player::handleSelfAction(ClientGameTimeAction a) {

	//start of movement logic
	direction = a.facingDirection;
	
	switch(a.movement) {
		case FORWARD :
			moveForward();
		// is this right allen?
		case BACKWARD:
				break;
		case LEFT:
				break;
		case RIGHT:
				break;
		case BACKWARD_LEFT:
		case BACKWARD_RIGHT:
		case FORWARD_LEFT:
		case FORWARD_RIGHT:
		case NONE:
		default:
			throw "Oh... something got fucked up in player handleSelfAction";

	}

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
