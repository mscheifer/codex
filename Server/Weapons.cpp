#include "Weapons.h"


Weapons::Weapons(void)
{
}


Weapons::~Weapons(void)
{
}

Weapons::Weapons(int dam, int ran, Coordinate d)
{
	damage = dam;
	range = ran;
	position = d;

}
