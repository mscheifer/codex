#include "Weapon.h"


Weapon::Weapon()
{
	Cool_Down_Time = 2000;
	Cool_Down_Counter = sf::Clock();
}


Weapon::~Weapon()
{
}

Weapon::Weapon(int dam, int ran, Coordinate d)
{
	Cool_Down_Counter = sf::Clock();
	Cool_Down_Time = 2000;
	damage = dam;
	range = ran;
	position = d;
	

}


void Weapon::handleAction(ClientGameTimeAction a){

}

bool Weapon::useWeapon() {
	if( canUseWeapon())  {
		//owner.subtractshit();
	}
	return false;
}

bool Weapon::canUseWeapon() {
	if( Cool_Down_Counter.getElapsedTime().asMilliseconds() < Cool_Down_Time /* && some other attributes*/) {
		return true;
	}
	return false;
}