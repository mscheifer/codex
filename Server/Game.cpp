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
  world.getPlayers()[minotaur]->setAsMinotaur(true);
}

int Game::join()
{
  unsigned int userID = world.getPlayers().size();
  Player * newPlayer = new Player(world.getRespawnPosition(userID), userID, &world);
  world.addPlayer(newPlayer);
  return userID;
}

void Game::evaluate(ClientGameTimeAction a) {
	std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();
	
	for( unsigned int i = 0; i <  currentPlayers.size(); i++ ) {
		currentPlayers[i]->handleAction(a);
    world.separatePlayers(currentPlayers[i]);
	}
}

/* updates & resolve collision for each clock tick */
void Game::updateAndResolveCollision() {
  std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();
  std::vector<Projectile*> currentProjectile = world.getLiveProjectTile();

  for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
		currentPlayers[i]->update();
	}

  for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		currentEntities[i]->update();
	}

  for( unsigned int i = 0; i < currentProjectile.size(); i++ ) {
		currentProjectile[i]->update();
	}

  //run collision fix here
  for( unsigned int i = 0; i <  currentPlayers.size(); i++ ) {
    currentPlayers[i]->handleCollisions();
	}
   
	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		currentEntities[i]->handleCollisions();
	}

  for( unsigned int i = 0; i < currentProjectile.size(); i++ ) {
		currentProjectile[i]->handleCollisions();
	}
}

ServerGameTimeRespond Game::prepResponse() {
	ServerGameTimeRespond s;
	s.weapons.clear();
	s.projectiles.clear();
	s.walls.clear();
	s.powerups.clear();
  s.players.clear();
	std::vector<Player*>  currentPlayers =  world.getPlayers();
	std::vector<Entity*> currentEntities = world.getEntity();
  std::vector<Projectile*> currentProjectiles = world.getLiveProjectTile();
	for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
     /*for testing
     if (currentPlayers[i]->getHealth() > 0) {
       (currentPlayers[i])->setHealth(currentPlayers[i]->getHealth()-1);
     } else  (currentPlayers[i])->setHealth(100);
     */
		 s.players.push_back(*currentPlayers[i]); //add the player to the return struct
	}

  for( unsigned int i = 0; i < currentProjectiles.size(); i++ ) {
    if(currentProjectiles[i]->canRender())
      s.projectiles.push_back(currentProjectiles[i]); 
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
     if (!currentPlayers[i]->isMinotaur()) {
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
  std::vector<Player*> players = world.getPlayers();
  for(auto playerIt = players.begin(); playerIt != players.end(); playerIt++) {
    if( (*playerIt)->player_id == playerId ) //if id match
      return InitPacket((*playerIt)->player_id, (*playerIt)->getPosition(), (*playerIt)->getDirection());
  }

  std::cout << "ERROR init packet, player id not found" << std::endl;
  return InitPacket(0, v3_t(0,0,0), v3_t(0,0,0));
}

void Game::clearEvents(){
  std::vector<Player *> currentPlayers =  world.getPlayers();
	std::vector<Entity *> currentEntities = world.getEntity();

  for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
    currentPlayers[i]->clearEvents();
	}
   
	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		currentEntities[i]->clearEvents();
	}
}
