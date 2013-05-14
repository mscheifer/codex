#include "Weapon.h"
#include "Projectile.h"

Weapon::Weapon(Map* m)
{
	Range_Cool_Down_Time = 0;
	Melee_Cool_Down_Time = 0;
	mpCost = 0;
	Range_Cool_Down_Counter = sf::Clock();
	Melee_Cool_Down_Counter = sf::Clock();
	this->map = m;
}


Weapon::~Weapon()
{
}

Weapon::Weapon(float damage, float ran, v3_t pos, float mpcost, Map* m)
{
	Range_Cool_Down_Time = 0;
	Melee_Cool_Down_Time = 0;
	Range_Cool_Down_Counter = sf::Clock();
	Melee_Cool_Down_Counter = sf::Clock();
	strength = damage;
	range = ran;
	position = pos;
	mpCost = mpcost;
	projectileSpeed = 20.0; // pending removal
	projectileRange = 10; //pending removal
	projectileStrength = 1; //pending removal
	this->map = m;
}


void Weapon::handleAction(ClientGameTimeAction){

}

void Weapon::onCollision(Entity*) {

}

bool Weapon::canUseWeapon(bool range_attack) {
	if(		(range_attack && Range_Cool_Down_Counter.getElapsedTime().asMilliseconds() > Range_Cool_Down_Time)
		||	(!range_attack && Melee_Cool_Down_Counter.getElapsedTime().asMilliseconds() > Melee_Cool_Down_Time)){
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
Projectile* Weapon::attackRange(v3_t dir , v3_t pos)
{
	Projectile* pj = map->produceProjectile();
  dir.normalize();
  pj->setDirection(dir);
  dir.scale(projectileSpeed);
  pj->setVelocity(dir);
  pj->setPosition(pos);

	pj->setStrength(projectileStrength);
	pj->setRange(projectileRange); //TODO not sure what this is @allen @alvin

	return pj;
}

