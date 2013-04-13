#pragma once
#include "Entity.h"
#include "Player.h"

class Weapons : public Entity
{
public:
	Weapons(void);
	~Weapons(void);
	Weapons(int damage, int range, Coordinate d);
	int getRange(void);
	int getDamage(void);

private:
	int range;
	int damage;
	Player owner;
};

