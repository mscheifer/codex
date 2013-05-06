#pragma once
#include "Weapon.h"
class WeaponFist :
  public Weapon
{
public:
  WeaponFist(Coordinate, Map*);
  ~WeaponFist(void);
  bool attackMelee();
  Projectile* attackRange();
  bool pickUp();
  bool dropDown(Coordinate dropPosition);
};

