#include "Player.h"

Player::Player(void){}
Player::Player(int x, int y, int z)
{
  position.x = x;
  position.y = y;
  position.z = z;
  strength = 10;
  defense = 5;
  health = 100;
  maxHealth = 100;
}



Player::~Player(void)
{
}

Coordinate const * Player::getPosition(void){
	temp_coordinate = &position;
	return  temp_coordinate;
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

void Player::moveForward()
{

}

void Player::moveBackward()
{

}

