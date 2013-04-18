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
	bool canUseWeapon();
	bool useWeapon(void); //returns if you can actually use it or not
private:
	int Cool_Down_Time; //cool down time between uses in milliseconds
  float mpCost;
	sf::Clock Cool_Down_Counter;
};

