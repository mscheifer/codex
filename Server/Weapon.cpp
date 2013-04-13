#include "Weapon.h"


Weapon::Weapon()
{
}


Weapon::~Weapon()
{
}

Weapon::Weapon(int dam, int ran, Coordinate d)
{
	damage = dam;
	range = ran;
	position = d;

}


void Weapon::handleAction(ClientGameTimeAction a){
}
