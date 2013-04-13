#pragma once
#include "Entity.h"
#include "Player.h"

class Weapon : public Entity
{
public:
	Weapon(void);
	~Weapon(void);
	Weapon(int damage, int range, Coordinate d);
	int getRange(void);
	int getDamage(void);
	void handleAction(ClientGameTimeAction a);

private:
	int range;
	int damage;
	Player owner;
};

