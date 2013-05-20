#pragma once
#include "Weapon.h"
class WeaponFist :
  public Weapon
{
public:
  static const WeaponType wtype = FIST;

  WeaponFist(v3_t, Map*);
  ~WeaponFist(void);
  bool attackMelee();
  Projectile* attackRange(v3_t dir, v3_t pos);
  //bool pickUp();
  //bool dropDown(v3_t dropPosition);

  WeaponType getWeaponType() const{ return wtype; }
};

