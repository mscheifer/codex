#include "Map.h"
#include "Projectile.h"
#include "Player.h"
#include "Entity.h"

const float Map::Item_Pick_Up_Ranges = 1.0f;


//TODO the rectangle should be the actual world bounds
Map::Map(void):q(0,Rectangle(gx::vector4(0,0,0),1000,1000)),freeProjectiles()
{
	map_size = 15;
	freeProjectiles = new std::stack<Projectile *>();
	init = true;
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
   if(freeProjectiles->empty())
   {
     for(unsigned int i = 0; i < 20; i++)
     {
       freeProjectiles->push(new Projectile(this));
     }
   }
   Projectile* ret = freeProjectiles->top();
   freeProjectiles->pop();
   entities.push_back(ret);
   return ret;
 }

 void Map::destroyProjectile(Projectile * proj)
 {
   proj->setOwner(NULL);
   freeProjectiles->push(proj);
   // should probably use a hasmap soon
   for(unsigned int i = 0; i < entities.size(); i++) {
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

void Map::addToQtree(Entity* e){
  std::vector<BoundingObj*> vec = e->getBoundingObjs();
  for( auto it = vec.begin(); it != vec.end(); ++it){
	  q.insert(*it);
  }
}
 
void Map::removeFromQtree(Entity* e){
  std::vector<BoundingObj*> vec = e->getBoundingObjs();
  for( auto it = vec.begin(); it != vec.end(); ++it){
      q.remove(*it);
  }
}