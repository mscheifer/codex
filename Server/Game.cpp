#include "Game.h"


Game::Game(void) : world()
{
}


Game::~Game(void)
{
}

void Game::chooseMinotaur() 
{
  if(StringToNumber<int>(ConfigManager::configMap["enableMinotaur"]) == 0)
    return;
  srand(static_cast<unsigned int>(time(nullptr)));
  int minotaur = rand() % ConfigManager::numPlayers();
  world.getPlayers()[minotaur]->setAsMinotaur(true);
}

int Game::join()
{
  unsigned int userID = world.getPlayers().size();
  Player * newPlayer = new Player(world.getRespawnPosition(), userID, &world);
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
  currentProjectile = world.getLiveProjectTile();
  for( unsigned int i = 0; i < currentProjectile.size(); i++ ) {
    //if(currentProjectile[i]->live) //TODO (but not really) fix this to use live
		  currentProjectile[i]->handleCollisions();
	}

  //delete the projectiles marked for deletion
  for( unsigned int i = 0; i < currentProjectile.size(); i++ ) {
    if(!currentProjectile[i]->live)
		  world.destroyProjectile(currentProjectile[i]);
	}
}

void Game::prepResponse(ServerGameTimeRespond* sgtr) {
  sgtr->state = Game_State::PLAYING;
	sgtr->weapons.clear();
	sgtr->projectiles.clear();
	sgtr->walls.clear();
	sgtr->powerups.clear();
  sgtr->players.clear();
	std::vector<Player*>  currentPlayers =  world.getPlayers();
	std::vector<Entity*> currentEntities = world.getEntity();
  std::vector<Projectile*> currentProjectiles = world.getLiveProjectTile();
	
  unsigned int deadPlayers = 0;
  bool minotaurLose  = false;
  //determine who wins
  for (unsigned int i = 0; i< currentPlayers.size(); i++ ) {
     currentPlayers[i]->kills = world.kills[i];
     currentPlayers[i]->wins = world.wins[i];
     if (!currentPlayers[i]->isMinotaur()) {
        if (currentPlayers[i]->dead) {
          deadPlayers++;
        }
     } else if (currentPlayers[i]->dead) {
        minotaurLose = true;
     }
  }
  if (currentPlayers.size() > 1) {
    if (minotaurLose) {
      sgtr->state = CIVILIAN_WIN; 
    }
    if (deadPlayers == currentPlayers.size()-1 ) {
      sgtr->state = MANOTAUR_WIN;
    }
  }
	for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
     /*for testing
     if (currentPlayers[i]->getHealth() > 0) {
       (currentPlayers[i])->setHealth(currentPlayers[i]->getHealth()-1);
     } else  (currentPlayers[i])->setHealth(100);
     */
		 sgtr->players.push_back(*currentPlayers[i]); //add the player to the return struct
     //were not sending boxes over network, if you dont ahve this it will try to destroy the actual bb
     sgtr->players[i].setBoundingObjs(std::vector<BoundingObj*>()); 
  }

  for( unsigned int i = 0; i < currentProjectiles.size(); i++ ) {
    if(currentProjectiles[i]->canRender())
      sgtr->projectiles.push_back(currentProjectiles[i]);
  }

	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
    if( currentEntities[i]->canRender() ) {
      switch (currentEntities[i]->getType()) { // I am casting pointers but I don't care
      case WALL:
		    sgtr->walls.push_back((Wall*) currentEntities[i]); 
        break;
      case PROJECTILE:
		    sgtr->projectiles.push_back((Projectile*) currentEntities[i]); 
        break;
      case POWER_UP:
		    sgtr->powerups.push_back((PowerUp*) currentEntities[i]); 
        break;
      case WEAPON:
		    sgtr->weapons.push_back((Weapon*) currentEntities[i]); 
        break;        
      default:
        std::cout<<"Error for Entity type!!!!!!"<<std::endl;
        break;
      }
    }
  }
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

void Game::restartGame()
{
  world.mapReset();
}

void Game::initScores() {
  world.initScores();
}

void Game::assignName(std::string name , int id){
  world.assignName(name,id);
}