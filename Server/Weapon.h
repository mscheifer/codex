#pragma once

#include <SFML/System/Clock.hpp>
#include "DeadlyEntity.h"

class Projectile;

class Weapon : public DeadlyEntity
{
public:
	Weapon(Map*);
	~Weapon(void);
	Weapon(float damage, float range, Coordinate d, float mpcost, Map*);
	int getRange(void);
	int getDamage(void);
	Velocity projectileSpeed;
	float getMpCost(){ return mpCost; }
	void handleAction(ClientGameTimeAction a);
	void onCollision(Entity* e);
	bool canUseWeapon(bool range_attack);
	void useWeapon(bool range_attack);
	bool canPickUp() { return pickedUp; }
	virtual bool attackMelee(); 
	
	virtual Projectile* attackRange(Direction d, Coordinate c);
  virtual bool pickUp(){ return false; };
  virtual bool dropDown(Coordinate dropPosition){ position = dropPosition; return false; };
protected:
	int Range_Cool_Down_Time; //cool down time between uses in milliseconds
	int Melee_Cool_Down_Time; 
	float projectileStrength;
	Position projectileRange;
	float mpCost;
	bool pickedUp;
	sf::Clock Range_Cool_Down_Counter;
	sf::Clock Melee_Cool_Down_Counter;
};

