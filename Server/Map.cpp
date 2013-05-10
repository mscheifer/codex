#include "Map.h"
#include "Projectile.h"
#include "Player.h"
#include "Entity.h"
#include "Wall.h"

const float Map::Item_Pick_Up_Ranges = 1.0f;


//TODO the rectangle should be the actual world bounds
Map::Map(void): freeProjectiles(),q(0,Rectangle(BoundingObj::vec4_t(0,0,0),1000,1000))
{
	map_size = 15;
	freeProjectiles = new std::stack<Projectile *>();
  initWalls();
}

void Map::initWalls(void)
{
  Direction facingEast = Direction(1,0,0);
  Direction facingNorth = Direction(0,1,0);
  int width = 10;
  int height = 4; 
  int depth = 1;

  int wallX = 7;
  int wallY = 7;

  int centerX = 0;
  int centerY = 0;
  int i;
  int startingX;
  int startingXNeg;
  int startingY;
  int startingYNeg;
  int startingZ = height/2;

  
  // Create the top and bottom perimeter from left to right.
  for( i = 0,
    startingX = ((wallX*width)/-2)+(width/2)+centerX,
    startingY = ((wallY*width)/-2)+centerY,
    startingYNeg = ((wallY*width)/2)+centerY;
    i < wallX; i++, startingX += width )
  {
    Wall* topWall = new Wall(width, depth, height, Coordinate(startingX,startingY, startingZ, 0,0,0), facingNorth, this);
    Wall* bottomWall = new Wall(width, depth, height, Coordinate(startingX,startingYNeg, startingZ, 0,0,0), facingNorth, this);
    this->entities.push_back(topWall);
    this->entities.push_back(bottomWall);
  }
  // Create the left and right perimeter from bottom up
  for( i = 0,
    startingX = ((wallX*width)/-2)+centerX,
    startingXNeg = ((wallX*width)/2)+centerX,
    startingY = ((wallY*width)/-2)+(width/2)+centerY;
    i < wallY; i++, startingY += width )
  {
    Wall* leftWall = new Wall(width, depth, height, Coordinate(startingX,startingY, startingZ, 0,0,0), facingEast, this);
    Wall* rightWall = new Wall(width, depth, height, Coordinate(startingXNeg,startingY, startingZ, 0,0,0), facingEast, this);
    this->entities.push_back(leftWall);
    this->entities.push_back(rightWall);
  }
  

  //Wall * floor = new Wall(10, 1, 1, Coordinate(0,0,0,0,0,0), facingEast, this);
  //this->entities.push_back(floor);

  //Wall * floor = new Wall(100, 100, 100, Coordinate(100,100,-20, 0,0,0), facingEast, this);
  //this->entities.push_back(floor);
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
   addToQtree(ret);
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
    (*it)->setQuadtree(&q);
    //q.insert(*it);
  }
  e->updateBounds();
}
 
void Map::removeFromQtree(Entity* e){
  std::vector<BoundingObj*> vec = e->getBoundingObjs();
  for( auto it = vec.begin(); it != vec.end(); ++it){
      q.remove(*it);
  }
}
