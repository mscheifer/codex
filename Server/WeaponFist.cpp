#include "WeaponFist.h"


WeaponFist::WeaponFist(Coordinate position):Weapon(10, 1, position, 0)
{ // Will inherit weapon's default constructor
}


WeaponFist::~WeaponFist(void)
{
}
bool WeaponFist::attackMelee()
{
}
bool WeaponFist::attackRange()
{
  attackMelee();
}
