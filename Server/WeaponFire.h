#pragma once
#include "Weapon.h"
class WeaponFire :
	public Weapon
{
public:
	WeaponFire(void);
	WeaponFire(Coordinate c);
	~WeaponFire(void);
};

