#pragma once
#include "Weapon.h"
class WeaponFire :
	public Weapon
{
public:
	WeaponFire(v3_t c, Map *);
	~WeaponFire(void);
};

