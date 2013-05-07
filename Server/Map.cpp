#include "Map.h"
#include "Projectile.h"
#include "Player.h"
#include "Entity.h"
#include "Wall.h"

const float Map::Item_Pick_Up_Ranges = 1.0f;


//TODO the rectangle should be the actual world bounds
Map::Map(void):q(0,Rectangle(gx::vector4(0,0,0),1000,1000)),freeProjectiles()
{
	map_size = 15;
	freeProjectiles = new std::stack<Projectile *>();
  initWalls();
}

void Map::initWalls(void)
{
  Direction facingEast = Direction(1,0,0);
  Direction facingNorth = Direction(0,1,0);
  unsigned int width = 100;
  unsigned int height = 40; 
  unsigned int depth = 10;
  unsigned int wallX = 15;
  unsigned int wallY = 15;

  unsigned int centerX = 0;
  unsigned int centerY = 0;
  unsigned int i;
  int startingX;
  int startingY;
  int startingZ = height/2;
  // Create the top and bottom perimeter from left to right.
  for( i = 0,
    startingX = ((wallX*width)/-2)+(width/2)+centerX,
    startingY = ((wallY*width)/2)+(width/2)+centerY;
    i < wallX; i++, startingX += width )
  {
    Wall* topWall = new Wall(width, depth, height, Coordinate(startingX,startingY, startingZ, 0,0,0), facingNorth);
    Wall* bottomWall = new Wall(width, depth, height, Coordinate(startingX,-startingY, startingZ, 0,0,0), facingNorth);
    this->entities.push_back(topWall);
    this->entities.push_back(bottomWall);
  }
  // Create the left and right perimeter from bottom up
  for( i = 0,
    startingX = ((wallX*width)/2)+(width/2)+centerX,
    startingY = ((wallY*width)/-2)+(width/2)+centerY;
    i < wallY; i++, startingY += width )
  {
    Wall* leftWall = new Wall(width, depth, height, Coordinate(startingX,startingY, startingZ, 0,0,0), facingEast);
    Wall* rightWall = new Wall(width, depth, height, Coordinate(-startingX,startingY, startingZ, 0,0,0), facingEast);
    this->entities.push_back(leftWall);
    this->entities.push_back(rightWall);
  }
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
   entities.push_back(ret); //note shouldn't this be live projecties?
   return ret;
 }

 //note should we just process all live projectiles, thenw e can remove
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
