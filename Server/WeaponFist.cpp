#include "WeaponFist.h"


WeaponFist::WeaponFist(Coordinate position, Map * m):Weapon(10, 1, position, 0, m)
{ // Will inherit weapon's default constructor
}


WeaponFist::~WeaponFist(void)
{
}
bool WeaponFist::attackMelee()
{
  return false;
}
Projectile* WeaponFist::attackRange()
{

  return NULL;
  //attackMelee();
}

bool WeaponFist::pickUp()
{
  if(pickedUp)
    return false;
  pickedUp = true;
  return true;
}
bool WeaponFist::dropDown(Coordinate dropPosition)
{
  if(pickedUp)
    return false;
  position = dropPosition;
  pickedUp = false;
  return true;
};