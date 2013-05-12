#pragma once
#include "Weapon.h"
class WeaponFist :
  public Weapon
{
public:
  WeaponFist(v3_t, Map*);
  ~WeaponFist(void);
  bool attackMelee();
  Projectile* attackRange();
  bool pickUp();
  bool dropDown(v3_t dropPosition);
};

