#include "Game.h"


Game::Game(void) : world()
{
}


Game::~Game(void)
{
}

void Game::chooseMinotaur() 
{
  srand(static_cast<unsigned int>(time(nullptr)));
  int minotaur = rand() % ConfigManager::numPlayers();
  world.getPlayers()[minotaur]->minotaur=true;
}

int Game::join()
{
  unsigned int userID = world.getPlayers().size();
  Player * newPlayer = new Player(v3_t(0,0,0), 0, &world);
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
}

/* updates & resolve collision for each clock tick */
void Game::updateAndResolveCollision() {
  std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();

  for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
		 //std::cout << " hello nigga, updating entities" << std::endl;
		 currentPlayers[i]->update();
     world.separatePlayers(currentPlayers[i]);
	}

  for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		 currentEntities[i]->update();
	}

  //run collision fix here
  for( unsigned int i = 0; i <  currentPlayers.size(); i++ ) {
    currentPlayers[i]->handleCollisions();
	}
   
	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		currentEntities[i]->handleCollisions();
	}
}

ServerGameTimeRespond Game::prepResponse() {
	ServerGameTimeRespond s;
	s.weapons.clear();
	s.projectiles.clear();
	s.walls.clear();
	s.powerups.clear();
  s.players.clear();
	std::vector<Player*> currentPlayers =  world.getPlayers();
	std::vector<Entity*> currentEntities = world.getEntity();
	for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
		 s.players.push_back(*currentPlayers[i]); //add the player to the return struct
	}

	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
    if( currentEntities[i]->canRender() ) {
      switch (currentEntities[i]->getType()) { // I am casting pointers but I don't care
      case WALL:
		    s.walls.push_back((Wall*) currentEntities[i]); 
        break;
      case PROJECTILE:
		    s.projectiles.push_back((Projectile*) currentEntities[i]); 
        break;
      case POWER_UP:
		    s.powerups.push_back((PowerUp*) currentEntities[i]); 
        break;
      case WEAPON:
		    s.weapons.push_back((Weapon*) currentEntities[i]); 
        break;        
      default:
        std::cout<<"Error for Entity type!!!!!!"<<std::endl;
        break;
      }
    }  
  }
	  
	unsigned int deadPlayers = 0;
  bool minotaurLose  = false;
  //determine who wins
  for (unsigned int i = 0; i< currentPlayers.size(); i++ ) {
     if (!currentPlayers[i]->minotaur) {
        if (currentPlayers[i]->dead) {
          deadPlayers++;
        }
     } else if (currentPlayers[i]->dead) {
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

InitPacket Game::getInitPacket(int playerId) {
  Player* player = nullptr;
  std::vector<Player*> players = world.getPlayers();
  for(auto playerP = players.begin(); playerP != players.end(); playerP++) {
    if( (*playerP)->player_id == playerId ){
      player = *playerP;
      return InitPacket(player->player_id, player->getPosition(), player->getDirection());
    }
  }

  std::cout << "ERROR init packet, player id not found" << std::endl;
  return InitPacket(player->player_id, player->getPosition(), player->getDirection());
}