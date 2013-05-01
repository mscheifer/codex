#include "Game.h"


Game::Game(void)
{

}


Game::~Game(void)
{
}

void Game::chooseMinotaur() 
{
  srand(time(NULL));
  int minotaur = rand() % NUM_PLAYERS;
  world.getPlayers()[minotaur]->minotaur=true;
}

int Game::join()
{
  int userID = world.getPlayers().size();
  Player * newPlayer = new Player(0,0,0,0);
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
  int deadPlayers=0;
  bool minotaurLose  = false;
  //determine who wins
  for (unsigned int i = 0; i< currentPlayers.size(); i++ ) {
     if (!currentPlayers[i]->minotaur) {
        if (currentPlayers[i]->dead) 
          deadPlayers++;
     }
     else if (currentPlayers[i]->dead) {
        minotaurLose = true;
     }
  }
  if (minotaurLose) {
    s.state = CIVILIAN_WIN; 
  }
  if (deadPlayers == currentPlayers.size()-1 ) {
    s.state = MANOTAUR_WIN;
  }
	/*for(int i = 0; i < currentEntities.size(); i++ ) {
		 currentEntities[i]->handleAction(a);
		 s.entities[i] = *currentEntities[i]; //add the player to the return struct
	}*/
	
	return s;
}
