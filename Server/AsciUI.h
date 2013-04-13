#pragma once
#include "Map.h"
#include "Entity.h"
#include "Player.h"
class AsciUI
{
public:
	AsciUI(void);
	~AsciUI(void);
	int update(Map*);
	int render(Player*);
};

