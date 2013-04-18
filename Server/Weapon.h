#pragma once
#include "Entity.h"
#include "Player.h"
#include <SFML/System/Clock.hpp>


class Weapon : public Entity
{
public:
	Weapon(void);
	~Weapon(void);
	Weapon(int damage, int range, Coordinate d);
	int getRange(void);
	int getDamage(void);
	void handleAction(ClientGameTimeAction a);
	bool canUseWeapon();
	bool useWeapon(void); //returns if you can actually use it or not
private:
	Player owner;
	int range;
	int damage;
	int Cool_Down_Time; //cool down time between uses in milliseconds
	sf::Clock Cool_Down_Counter;
};

