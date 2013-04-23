#include "Weapon.h"


Weapon::Weapon()
{
	Range_Cool_Down_Time = 0;
  Melee_Cool_Down_Time = 0;
  mpCost = 0;
	Range_Cool_Down_Counter = sf::Clock();
  Melee_Cool_Down_Counter = sf::Clock();
}


Weapon::~Weapon()
{
}

Weapon::Weapon(float damage, float ran, Coordinate d, float mpcost)
{
	Range_Cool_Down_Time = 0;
  Melee_Cool_Down_Time = 0;
  Range_Cool_Down_Counter = sf::Clock();
  Melee_Cool_Down_Counter = sf::Clock();
	strength = damage;
	range = ran;
	position = d;
  mpCost = mpcost;
}


void Weapon::handleAction(ClientGameTimeAction a){

}

bool Weapon::canUseWeapon() {
	if( Range_Cool_Down_Counter.getElapsedTime().asMilliseconds() < Range_Cool_Down_Time
      && Melee_Cool_Down_Counter.getElapsedTime().asMilliseconds() < Melee_Cool_Down_Time) {
		return true;
	}
	return false;
}