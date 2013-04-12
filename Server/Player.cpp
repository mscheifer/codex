#include "Player.h"


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

bool Player::attack(Entity *other)
{
  if(Player *otherPlayer = dynamic_cast<Player *>(other))
  {
    return damage(otherPlayer);
  }
  else
    return false;
}

bool Player::damage(Player *otherPlayer)
{
  if(otherPlayer->health <= 0)
    return false;
  int damage = strength-otherPlayer->defense;
  damage = ( damage > 0? damage: 0);
  int newHealth = otherPlayer->health - damage;
  otherPlayer->health = (newHealth > 0 ? newHealth : 0);
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

