#include "Weapon.h"
#include "Projectile.h"

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

void Weapon::onCollision(Entity e) {

}

bool Weapon::canUseWeapon(bool range_attack) {
	if(		(range_attack && Range_Cool_Down_Counter.getElapsedTime().asMilliseconds() < Range_Cool_Down_Time)
		||	(!range_attack && Melee_Cool_Down_Counter.getElapsedTime().asMilliseconds() < Melee_Cool_Down_Time)){
			return true;
	}
	return false;
}

void Weapon::useWeapon( bool range_attack){
	if(range_attack) {
		Range_Cool_Down_Counter.restart();
	} else {
		Melee_Cool_Down_Counter.restart();
	}
}

bool Weapon::attackMelee()
{
	return false;
}
Projectile* Weapon::attackRange()
{
	Projectile* pj; // somehow get it 

	pj->setStrength(projectileStrength);


	return pj;
}

