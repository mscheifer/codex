#pragma once
#include "weapon.h"
class WeaponFist :
  public Weapon
{
public:
  WeaponFist(Coordinate);
  ~WeaponFist(void);
  bool attackMelee();
  Projectile* attackRange();
};

