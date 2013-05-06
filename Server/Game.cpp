#include "Game.h"


Game::Game(void)
{
	world = Map();
}


Game::~Game(void)
{
}

void Game::chooseMinotaur() 
{
  srand(static_cast<unsigned int>(time(NULL)));
  int minotaur = rand() % NUM_PLAYERS;
  world.getPlayers()[minotaur]->minotaur=true;
}

int Game::join()
{
  unsigned int userID = world.getPlayers().size();
  Player * newPlayer = new Player(0,0,0,0, &world);
  newPlayer->player_id = userID;
  world.addPlayer(newPlayer);
  return userID;
}

void Game::evaluate(ClientGameTimeAction a) {
	std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();
	

	for( unsigned int i = 0; i <  currentPlayers.size(); i++ ) {
		 currentPlayers[i]->handleAction(a);
	}

	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		 std::cout << " hello nigga, updating entities" << std::endl;
		 currentEntities[i]->update();
	}
	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		currentEntities[i]->onCollision();
	}

	//run collision fix here


	
}

ServerGameTimeRespond Game::prepResponse() {
	ServerGameTimeRespond s;
	s.entities.clear();
	std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();
	for( unsigned int i = 0; i <  currentPlayers.size(); i++ ) {
		 s.players[i] = *currentPlayers[i]; //add the player to the return struct
	}

	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		s.entities.push_back(currentEntities[i]); //add the player to the return struct
	}
	  
	unsigned int deadPlayers = 0;
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

  return s;
}