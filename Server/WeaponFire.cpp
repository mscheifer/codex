#include "WeaponFire.h"


WeaponFire::WeaponFire(void)
{
}


WeaponFire::~WeaponFire(void)
{
}

WeaponFire::WeaponFire(Coordinate c):Weapon(2.0,300, c, 20.0) {

	Range_Cool_Down_Time = 500;
	Melee_Cool_Down_Time = 200;
	mpCost = 20;

}