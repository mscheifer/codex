#pragma once
#include "Weapon.h"
class WeaponFist :
  public Weapon
{
public:
  static const WeaponType wtype = FIST;
  static const bool hasRangedAttack = false;
  static const bool hasMeleeAttack = true;

  WeaponFist(v3_t, Map*);
  ~WeaponFist(void);
  //bool pickUp();
  //bool dropDown(v3_t dropPosition);

  WeaponType getWeaponType() const{ return wtype; }
  bool getHasRangedAttack() const { return hasRangedAttack; }
  bool getHasMeleeAttack() const { return hasMeleeAttack; }
};

