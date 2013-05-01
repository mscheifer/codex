#include "Map.h"
#include "Projectile.h"
#include "Player.h"
#include "Entity.h"

const float Map::Item_Pick_Up_Ranges = 1.0f;


Map::Map(void)
{
	map_size = 15;
	
}


Map::~Map(void)
{
}

std::vector<Entity *> Map::getEntity() {
	 return entities;
}
  
 std::vector<Player *> Map::getPlayers(){
	 return players;
 }

 Projectile* Map::produceProjectile()
 {
   if(freeProjectiles.empty())
     return NULL;
   Projectile* ret = freeProjectiles.top();
   freeProjectiles.pop();
   entities.push_back(ret);
   return ret;
 }

 void Map::destroyProjectile(Projectile * proj)
 {
   proj->setOwner(NULL);
   freeProjectiles.push(proj);
   // should probably use a hasmap soon
   for(int i = 0; i < entities.size(); i++) {
	   if(entities.at(i) == proj) {
			entities.erase(entities.begin() + i);
	   }
   }
 }

 bool Map::addPlayer(Player * newPlayer)
 {
   players.push_back(newPlayer);
   return true;
 }
