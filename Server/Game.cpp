#include "Game.h"


Game::Game(void)
{
}


Game::~Game(void)
{
}

ServerGameTimeRespond Game::evaluate(ClientGameTimeAction a) {
	
	ServerGameTimeRespond s;

  std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();

	for(int i = 0; i <  currentPlayers.size(); i++ ) {
		 currentPlayers[i]->handleAction(a);
		 s.players[i] = *currentPlayers[i]; //add the player to the return struct
	}
	for(int i = 0; i < currentEntities.size(); i++ ) {
		 currentEntities[i]->handleAction(a);
		 s.entities[i] = *currentEntities[i]; //add the player to the return struct
	}
	
	return s;
}
