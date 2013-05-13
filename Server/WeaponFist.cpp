#include "WeaponFist.h"


WeaponFist::WeaponFist(v3_t pos, Map * m):Weapon(10, 1, pos, 0, m)
{ // Will inherit weapon's default constructor
  // will it? --Matt
  //TODO it won't fix if needed @alvin @allen
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

bool WeaponFist::dropDown(v3_t dropPosition)
{
  if(pickedUp)
    return false;
  position = dropPosition;
  pickedUp = false;
  return true;
};
