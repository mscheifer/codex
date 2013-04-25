#pragma once
#include "weapon.h"
class WeaponFist :
  public Weapon
{
public:
  WeaponFist(Coordinate);
  ~WeaponFist(void);
  bool attackMelee();
  bool attackRange();
  bool pickUp();
  bool dropDown(Coordinate dropPosition);
};

