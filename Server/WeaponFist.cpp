#include "WeaponFist.h"

WeaponFist::WeaponFist(v3_t pos, Map * m):Weapon(10, 1, pos, 0, m)
{
}

WeaponFist::~WeaponFist(void)
{
}

/*
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
*/