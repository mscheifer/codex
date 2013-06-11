#include "Game.h"


Game::Game(void) : world()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}


Game::~Game(void)
{
}

void Game::chooseMinotaur() 
{
  if(StringToNumber<int>(ConfigManager::configMap["enableMinotaur"]) == 0)
    return;

  std::vector<unsigned int> playerIds;
  for( unsigned int i = 0; i < ConfigManager::numPlayers(); i++){
    playerIds.push_back(i);
  }

  unsigned int numMino = StringToNumber<unsigned int>(ConfigManager::configMap["numMinotaur"]);
  std::vector<unsigned int> selectedMinos;
  for( unsigned int i = 0, j = 0; i < numMino; i++, j++){
    int mino = rand() % (playerIds.size()-j);
    selectedMinos.push_back(playerIds[mino]);
    //switch the values
    int temp = playerIds[playerIds.size()-j-1];
    playerIds[playerIds.size()-j-1] = playerIds[mino];
    playerIds[mino] = temp;
  }

  for( unsigned int i = 0; i < numMino; i++){
    world.getPlayers()[selectedMinos[i]]->setAsMinotaur(true);
  }
  //int minotaur = rand() % ConfigManager::numPlayers();
}

int Game::join()
{
  unsigned int userID = world.getPlayers().size();
  Player * newPlayer = new Player(world.getRespawnPosition(), userID, &world);
  world.addPlayer(newPlayer);
  return userID;
}

void Game::evaluate(ClientGameTimeAction a) {
	const std::vector<Player *>& currentPlayers =  world.getPlayers();
	const std::vector<Entity *>& currentEntity = world.getEntity();
	
	for( unsigned int i = 0; i <  currentPlayers.size(); i++ ) {
		currentPlayers[i]->handleAction(a);
    world.separatePlayers(currentPlayers[i]);
	}
}

/* updates & resolve collision for each clock tick */
void Game::updateAndResolveCollision() {
  const std::vector<Player *>& currentPlayers =  world.getPlayers();
	const std::vector<Entity *>& currentEntities = world.getEntity();
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

void Game::prepResponse(ServerGameTimeRespond& sgtr) {
  sgtr.state = Game_State::PLAYING;
  sgtr.weapons.clear();
	sgtr.projectiles.clear();
	sgtr.walls.clear();
	sgtr.powerups.clear();
  sgtr.players.clear();
	const std::vector<Player *>&  currentPlayers =  world.getPlayers();
	const std::vector<Entity *>& currentEntities = world.getEntity();
  const std::vector<Projectile*>& currentProjectiles = world.getLiveProjectTile();
	
  unsigned int deadMinotaur = 0;
  unsigned int deadPlayers = 0;
  //determine who wins
  for (unsigned int i = 0; i< currentPlayers.size(); i++ ) {
     currentPlayers[i]->kills = world.kills[i];
     currentPlayers[i]->wins = world.wins[i];

     if (currentPlayers[i]->dead) {
        if (currentPlayers[i]->isMinotaur()) {
          deadMinotaur++;
        } else {
          deadPlayers++;
        }
     } 
  }

  int numMino = StringToNumber<int>(ConfigManager::configMap["numMinotaur"]);
  int numPlayers = StringToNumber<int>(ConfigManager::configMap["players"]);
  std::cout << "dead mino " << deadMinotaur << " numMino " << numMino << " dead players " << deadPlayers << " numPlayers " << numPlayers-numMino << std::endl;

  if (currentPlayers.size() > 1) {
    if (deadMinotaur == numMino) {
      sgtr.state = CIVILIAN_WIN; 
    }
    if (deadPlayers == numPlayers-numMino) {
      sgtr.state = MANOTAUR_WIN;
    }
    for (unsigned int i = 0; i < currentPlayers.size() ; i++ ) {
      if((!currentPlayers[i]->isMinotaur() && sgtr.state == CIVILIAN_WIN)
      || (currentPlayers[i]->isMinotaur() && sgtr.state == MANOTAUR_WIN)) {
        world.wins[i]++;
        currentPlayers[i]->wins++;
      }
    }
  }
	for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
		 sgtr.players.push_back(*currentPlayers[i]); //add the player to the return struct
     //were not sending boxes over network, if you dont ahve this it will try to destroy the actual bb
     sgtr.players[i].setBoundingObjs(std::vector<BoundingObj*>()); 
  }

  for( unsigned int i = 0; i < currentProjectiles.size(); i++ ) {
    if(currentProjectiles[i]->canRender()) {
      sgtr.projectiles.push_back(currentProjectiles[i]);
    }
  }

	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
    if( currentEntities[i]->canRender() ) {
      switch (currentEntities[i]->getType()) { // I am casting pointers but I don't care
      case WALL:
		    sgtr.walls.push_back(static_cast<Wall*>(currentEntities[i])); 
        break;
      case PROJECTILE:
		    sgtr.projectiles.push_back(static_cast<Projectile*>(currentEntities[i])); 
        break;
      case POWER_UP:
		    sgtr.powerups.push_back(static_cast<PowerUp*>(currentEntities[i])); 
        break;
      case WEAPON:
		    sgtr.weapons.push_back(static_cast<Weapon*>(currentEntities[i])); 
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
      return InitPacket((*playerIt)->player_id, (*playerIt)->getPosition(), (*playerIt)->getDirection(), world.getStaticEntities());
  }

  std::cout << "ERROR init packet, player id not found" << std::endl;
  return InitPacket(0, v3_t(0,0,0), v3_t(0,0,0), world.getStaticEntities());
}

void Game::clearEvents(){
  const std::vector<Player *>& currentPlayers =  world.getPlayers();
	const std::vector<Entity *>& currentEntities = world.getEntity();
  const std::vector<Projectile*>& currentProjectiles = world.getLiveProjectTile();

  for( unsigned int i = 0; i < currentPlayers.size(); i++ ) {
    currentPlayers[i]->clearEvents();
	}
   
	for( unsigned int i = 0; i < currentEntities.size(); i++ ) {
		currentEntities[i]->clearEvents();
	}
  for( unsigned int i = 0; i < currentProjectiles.size(); i++ ) {
    //if(currentProjectile[i]->live) //TODO (but not really) fix this to use live
		  currentProjectiles[i]->clearEvents();
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