#pragma once
#include "Entity.h"
#include <SFML/System/Clock.hpp>
#include "DeadlyEntity.h"


class Weapon : public DeadlyEntity
{
public:
	Weapon(void);
	~Weapon(void);
	Weapon(float damage, float range, Coordinate d, float mpcost);
	int getRange(void);
	int getDamage(void);
	float getMpCost(){ return mpCost; }
	void handleAction(ClientGameTimeAction a);
	void onCollision(Entity e);
	bool canUseWeapon(bool range_attack);
	void useWeapon(bool range_attack);
	bool canPickUp() { return pickedUp; }
	virtual bool attackMelee();
	virtual bool attackRange();
protected:
	int Range_Cool_Down_Time; //cool down time between uses in milliseconds
	int Melee_Cool_Down_Time; 
	float mpCost;
	bool pickedUp;
	sf::Clock Range_Cool_Down_Counter;
	sf::Clock Melee_Cool_Down_Counter;
};

