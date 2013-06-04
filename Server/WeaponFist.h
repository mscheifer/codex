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
  //this needs to be here, or it will use the inherited one, 
  //we want fist to have no bounding box, so you can't pick it up
  void updateBounds(){}
  //bool pickUp();
  //bool dropDown(v3_t dropPosition);

  WeaponType getWeaponType() const{ return wtype; }
  bool getHasRangedAttack() const { return hasRangedAttack; }
  bool getHasMeleeAttack() const { return hasMeleeAttack; }
};

