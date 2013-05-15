#pragma once
#include "Weapon.h"
class WeaponFire :
	public Weapon
{
public:
  static const WeaponType wtype = FIRE;

	WeaponFire(v3_t c, Map *);
	~WeaponFire(void);
  void updateBounds();
  WeaponType getWeaponType() const{ return wtype; }
};

