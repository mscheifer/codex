#include "Game.h"


Game::Game(void)
{

}


Game::~Game(void)
{
}

int Game::join()
{
  int userID = world.getPlayers().size();
  Player * newPlayer = new Player();
  newPlayer->player_id = userID;
  world.addPlayer(newPlayer);
  return userID;
}

ServerGameTimeRespond Game::evaluate(ClientGameTimeAction a) {
	
	ServerGameTimeRespond s;

  std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();

	for( unsigned int i = 0; i <  currentPlayers.size(); i++ ) {
		 currentPlayers[i]->handleAction(a);
		 s.players[i] = *currentPlayers[i]; //add the player to the return struct
	}
	/*for(int i = 0; i < currentEntities.size(); i++ ) {
		 currentEntities[i]->handleAction(a);
		 s.entities[i] = *currentEntities[i]; //add the player to the return struct
	}*/
	
	return s;
}
