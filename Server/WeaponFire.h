#pragma once
#include "Weapon.h"
class WeaponFire :
	public Weapon
{
public:
	WeaponFire(Coordinate c, Map *);
	~WeaponFire(void);
};

