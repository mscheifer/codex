#include "Weapons.h"


Weapons::Weapons()
{
}


Weapons::~Weapons()
{
}

Weapons::Weapons(int dam, int ran, Coordinate d)
{
	damage = dam;
	range = ran;
	position = d;

}
