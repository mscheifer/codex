#include "Player.h"

Player::Player(void){}
Player::Player(int x, int y, int z)
{
  position.x = x;
  position.y = y;
  position.z = z;
  direction.x = 0.0;
  direction.y = -1.0;
  direction.z = 0.0;
  strength = 10;
  defense = 5;
  health = 100;
  maxHealth = 100;
}



Player::~Player(void)
{
}

Coordinate Player::getPosition(void){
  return position;
}

bool Player::attackBy(Player *other)
{
  if(other)
  {
    return damageBy(other);
  }
  else
    return false;
}

bool Player::damageBy(Player *otherPlayer)
{
  if(otherPlayer->health <= 0)
    return false;
  int damage = otherPlayer->strength - defense;
  damage = ( damage > 0? damage: 0);
  int newHealth = health - damage;
  health = (newHealth > 0 ? newHealth : 0);
  return true;
}

int Player::getHealth()
{
  return health;
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

std::string Player::getString()
{
  std::stringstream returnString;
  returnString<< " x="<<position.x<< " y="<<position.y<< " z="<<position.z<<std::endl;
  returnString<< " x="<<position.velocityX<< " y="<<position.velocityY<< " z="<<position.velocityZ<<std::endl;
  return returnString.str();
}