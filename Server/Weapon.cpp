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

Weapon::Weapon(float damage, float ran, Coordinate d, float mpcost, Map* m)
{
	Range_Cool_Down_Time = 0;
	Melee_Cool_Down_Time = 0;
	Range_Cool_Down_Counter = sf::Clock();
	Melee_Cool_Down_Counter = sf::Clock();
	strength = damage;
	range = ran;
	position = d;
	mpCost = mpcost;
	projectileSpeed = 2.0; // pending removal
	projectileRange = 300; //pending removal
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
Projectile* Weapon::attackRange(Direction d , Coordinate c)
{
	Projectile* pj = map->produceProjectile();
	//pj = Projectile(c,d);
  //TODO is the direction normalized?
  c.velocityX = d.x*projectileSpeed;
  c.velocityY = d.y*projectileSpeed;
  c.velocityZ = d.z*projectileSpeed;
	//c.velocityX = projectileSpeed;
	//c.velocityY = projectileSpeed;
	//c.velocityZ = projectileSpeed;
	
	pj->setPosition(c);
	pj->setDirection(d);
	pj->setStrength(projectileStrength);
	pj->setRange(projectileRange);

	return pj;
}

