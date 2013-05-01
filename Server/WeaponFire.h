#pragma once
#include "weapon.h"
class WeaponFire :
	public Weapon
{
public:
	WeaponFire(void);
	WeaponFire(Coordinate c);
	~WeaponFire(void);
};

