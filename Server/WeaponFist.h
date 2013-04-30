#pragma once
#include "Weapon.h"
class WeaponFist :
  public Weapon
{
public:
  WeaponFist(Coordinate);
  ~WeaponFist(void);
  bool attackMelee();
  Projectile* attackRange();
  bool pickUp();
  bool dropDown(Coordinate dropPosition);
};

