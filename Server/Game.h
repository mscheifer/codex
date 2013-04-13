#pragma once
#include "Map.h"
#include "ServerGameTimeRespond.h"
#include "ClientGameTimeAction.h"

class Game
{
public:
	Game(void);
	Game(Map m);
	ServerGameTimeRespond evaluate(ClientGameTimeAction a);
	~Game(void);
private:
	Map world;
};

