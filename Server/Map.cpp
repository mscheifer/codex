#include "Map.h"
#include "Projectile.h"
#include "Player.h"
#include "Entity.h"
#include "Wall.h"
#include "Weapon.h"
#include "WeaponFist.h"
#include "WeaponFire.h"
#include "PowerUp.h"

const float Map::Item_Pick_Up_Ranges = 1.0f;

//TODO the rectangle should be the actual world bounds
Map::Map(void): spawnPositions(), freeProjectiles(), q(0,Rectangle(BoundingObj::vec4_t(0,0,0),1000,1000))
{
	map_size = 15;
	freeProjectiles = new std::stack<Projectile *>();
  initWallsOne();
  initPowerUps();
}

void Map::mapReset()
{
  this->q.clear();
  spawnPositions.clear();
  entities.clear();
  liveProjectTile.clear();
  initWalls();
  initPowerUps();
  for(unsigned int i = 0; i < players.size(); i++)
  {
    players[i]->reset(this->getRespawnPosition(players[i]->player_id));
  }
}

void Map::initPowerUps() {
  PowerUp* superPower = new PowerUp(v3_t(2,2,0), this, ICE3DEBUFF);
  superPower->setRespownTime(5000);
  this->entities.push_back(superPower);

  PowerUp* p2 = new PowerUp(v3_t(2,5,0), this, THU2DEBUFF);
  p2->setRespownTime(5000);
  this->entities.push_back(p2);

  PowerUp* p3 = new PowerUp(v3_t(2,9,0), this, ICE1DEBUFF);
  p3->setRespownTime(5000);
  this->entities.push_back(p3);
}

void Map::initWallsOne(void)
{
  //WeaponFire* w1 = new WeaponFire(v3_t(100,100,0), this, FIR1);
  //w1->dropDown(v3_t(10,10,0));
  //w1->setDirection(v3_t(0,1,0));
  //entities.push_back(w1);
  //WeaponFire* w2 = new WeaponFire(v3_t(120,120,0), this, THU1);
  //w2->dropDown(v3_t(10,-10,0));
  //w2->setDirection(v3_t(0,1,0));
  //entities.push_back(w2);
  //WeaponFire* w3 = new WeaponFire(v3_t(120,120,0), this, ICE1);
  //w3->dropDown(v3_t(-10,-10,0));
  //w3->setDirection(v3_t(0,1,0));
  //entities.push_back(w3);

  spawnPositions.push_back(v3_t(4,-4,1));
  spawnPositions.push_back(v3_t(4,4,1));
  spawnPositions.push_back(v3_t(-4,-4,1));
  spawnPositions.push_back(v3_t(-4,4,1));

  v3_t facingEast(1,0,0);
  v3_t facingNorth(0,1,0);
  unsigned int width = 10;
  unsigned int height = 1; 
  unsigned int depth = 4;

  float wallX = 25;
  float wallY = 25;

  float centerX = 0;
  float centerY = 0;
  int i;
  float startingX;
  float startingXNeg;
  float startingY;
  float startingYNeg;
  float startingZ = depth/2.0f;

  // Create the top and bottom perimeter from left to right.
  for( i = 0,
    startingX = ((wallX*width)/-2)+(width/2)+centerX,
    startingY = ((wallY*width)/-2)+centerY,
    startingYNeg = ((wallY*width)/2)+centerY;
    i < wallX; i++, startingX += width )
  {
    Wall* topWall = new Wall(width, depth, height, 
      v3_t(startingX,startingY, startingZ), facingNorth, this);
    Wall* bottomWall = new Wall(width, depth, height, 
      v3_t(startingX,startingYNeg, startingZ), facingNorth, this);
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
    Wall* leftWall = new Wall(width, depth, height,
      v3_t(startingX,startingY, startingZ), facingEast, this);
    Wall* rightWall = new Wall(width, depth, height,
      v3_t(startingXNeg,startingY, startingZ), facingEast, this);
    this->entities.push_back(leftWall);
    this->entities.push_back(rightWall);
  }
    /*
  Wall *moveableWall = new Wall(width, depth, height, v3_t(0,0,20), facingNorth, this);
  moveableWall->setWallChangeTime(1000.f);
  moveableWall->addNewCenter(v3_t(0,0,5));
  moveableWall->addNewCenter(v3_t(0,0,20));
  moveableWall->addNewCenter(v3_t(0,10,20));
  moveableWall->addNewCenter(v3_t(0,0,20));
  this->entities.push_back(moveableWall);
  */
  Wall * floor = new Wall(1000, 10, 1000, v3_t(0,0,-5), facingEast, this);
  this->entities.push_back(floor);
  floor->setRender(false);

   // REMOVE THIS TO KILL GRAPHICS
  // Creating facing north walls. DO NOT TOUCH THIS
  int row1[] = {-1};
  int row2[] = {-1};
  int row3[] = {-1};
  int row4[] = {-1};
  int row5[] = {0,1,2,3,4,8,9,10,11,12,13,14,15,16,17,20,21,22,23,24, -1};
  int row6[] = {-1};
  int row7[] = {-1};
  int row8[] = {-1};
  int row9[] =  {3,4,5,6,7,9,10,11,14,15,16,18,19,20,21,22,-1};//
  int row10[] = { -1};
  int row11[] = {-1};
  int row12[] = {-1};
  int row13[] = {-1};
  int row14[] = { -1};
  int row15[] = {-1};
  int row16[] = {3,4,5,6,7,9,10,11,14,15,16,18,19,20,21,22,-1};//
  int row17[] = { -1};
  int row18[] = { -1};
  int row19[] = {-1};
  int row20[] = {0,1,2,3,4,8,9,10,11,12,13,14,15,16,17,20,21,22,23,24, -1};
  
  int * rows[] = {row1, row2, row3, row4, row5, row6, row7, row8, row9,
                  row10, row11, row12, row13, row14, row15, row16, row17,
                  row18, row19, row20};
 

  for( i = 0,
    startingX = ((wallX*width)/-2)+(width/2)+centerX,
    startingY = ((wallY*width)/2)-width+centerY;
    i < 20; i++, startingY -= width)
  {
   
    addWallDirection(startingX, startingY, startingZ, facingNorth, rows[i]);
  }

    
  int column1[] = {4,19,-1};
  int column2[] = {4,19,-1};
  int column3[] = {4,19,-1};
  int column4[] = {4,19,-1};
  int column5[] = {7,17,-1};
  int column6[] = {7,17,-1};
  int column7[] = {7,17,-1};
  int column8[] = {7,17,-1};
  int column9[] = {8,16,-1};
  int column10[] = {8,16,-1};
  int column11[] = {8,16,-1};
  int column12[] = {8,16,-1};
  int column13[] = {8,16,-1};
  int column14[] = {8,16,-1};
  int column15[] = {8,16,-1};
  int column16[] = {7,17,-1};
  int column17[] = {7,17,-1};
  int column18[] = {7,17,-1};
  int column19[] = {7,17,-1};
  int column20[] = {4,19,-1};
  int column21[] = {4,19,-1};
  int column22[] = {4,19,-1};
  int column23[] = {4,19,-1};
  int column24[] = {-1};

  
  int * columns[] = {column1, column2, column3, column4, column5, column6,
                     column7, column8, column9, column10, column11, column12,
                     column13, column14, column15, column16, column17, column18, column19, column20, column21, column22, column23, column24 };
  for( i = 0,
    startingX = ((wallX*width)/-2)+width+centerX,
    startingY = ((wallY*width)/2)-(width*1.5f)+centerY;
    i < 24; i++, startingY -= width)
  {
    addWallDirection(startingX, startingY, startingZ, facingEast, columns[i]);
  }
  
}

void Map::initWalls(void)
{
  //TODO move this
  //WeaponFire* w1 = new WeaponFire(v3_t(100,100,0), this, FIR1);
  //w1->dropDown(v3_t(10,10,0));
  //w1->setDirection(v3_t(0,1,0));
  //entities.push_back(w1);
  WeaponFire* w2 = new WeaponFire(v3_t(120,120,0), this, THU1);
  w2->dropDown(v3_t(10,-10,0));
  w2->setDirection(v3_t(0,1,0));
  entities.push_back(w2);
  //WeaponFire* w3 = new WeaponFire(v3_t(120,120,0), this, ICE1);
  //w3->dropDown(v3_t(-10,-10,0));
  //w3->setDirection(v3_t(0,1,0));
  //entities.push_back(w3);

  spawnPositions.push_back(v3_t(4,-4,1));
  spawnPositions.push_back(v3_t(4,4,1));
  spawnPositions.push_back(v3_t(-4,-4,1));
  spawnPositions.push_back(v3_t(-4,4,1));

  v3_t facingEast(1,0,0);
  v3_t facingNorth(0,1,0);
  unsigned int width = 10;
  unsigned int height = 1; 
  unsigned int depth = 4;

  float wallX = 7;
  float wallY = 7;

  float centerX = 0;
  float centerY = 0;
  int i;
  float startingX;
  float startingXNeg;
  float startingY;
  float startingYNeg;
  float startingZ = depth/2.0f;

  // Create the top and bottom perimeter from left to right.
  for( i = 0,
    startingX = ((wallX*width)/-2)+(width/2)+centerX,
    startingY = ((wallY*width)/-2)+centerY,
    startingYNeg = ((wallY*width)/2)+centerY;
    i < wallX; i++, startingX += width )
  {
    Wall* topWall = new Wall(width, depth, height, 
      v3_t(startingX,startingY, startingZ), facingNorth, this);
    Wall* bottomWall = new Wall(width, depth, height, 
      v3_t(startingX,startingYNeg, startingZ), facingNorth, this);
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
    Wall* leftWall = new Wall(width, depth, height,
      v3_t(startingX,startingY, startingZ), facingEast, this);
    Wall* rightWall = new Wall(width, depth, height,
      v3_t(startingXNeg,startingY, startingZ), facingEast, this);
    this->entities.push_back(leftWall);
    this->entities.push_back(rightWall);
  }

  Wall *moveableWall = new Wall(width, depth, height, v3_t(0,0,20), facingNorth, this);
  moveableWall->setWallChangeTime(1000.f);
  moveableWall->addNewCenter(v3_t(0,0,5));
  moveableWall->addNewCenter(v3_t(0,0,20));
  moveableWall->addNewCenter(v3_t(0,10,20));
  moveableWall->addNewCenter(v3_t(0,0,20));
  this->entities.push_back(moveableWall);
  Wall * floor = new Wall(1000, 10, 1000, v3_t(0,0,-5), facingEast, this);
  this->entities.push_back(floor);
  floor->setRender(false);

  return; // REMOVE THIS TO KILL GRAPHICS
  // Creating facing north walls. DO NOT TOUCH THIS

  int row1[] = {0,2,3,4,5,6,7,8,9,11,12,13, -1};
  int row2[] = {1,2,5,6,7,8,9,10,11,12,13,14,15, -1};
  int row3[] = {2,3,6,9,11,12,14,-1};
  int row4[] = {10,12,13, -1};
  int row5[] = {9,12,13, -1};
  int row6[] = {1,-1};
  int row7[] = {0,2,3,4,7,14, -1};
  int row8[] = {0,1,2,3,4,8,10,11,12,13, -1};
  int row9[] = {0,2,3,4,5,6,7,10,11,13, -1};
  int row10[] = {1,3,4,5,9,14, -1};
  int row11[] = {1,2,4,5,8,9,10,11,13,14, -1};
  int row12[] = {1,2,3,5,6,7,11,12,13, -1};
  int row13[] = {0,2,3,-1};
  int row14[] = {15, -1};
  int row15[] = {9,10,11,12, -1};
  int row16[] = {5,9,10, -1};
  int row17[] = {5,6,12,13, -1};
  int row18[] = {0,1,2,5,6,8,11,12,13, -1};
  int row19[] = {3,4,5,6,7,9,10,12,13,14, -1};
  int * rows[] = {row1, row2, row3, row4, row5, row6, row7, row8, row9,
                  row10, row11, row12, row13, row14, row15, row16, row17,
                  row18, row19};
  for( i = 0,
    startingX = ((wallX*width)/-2)+(width/2)+centerX,
    startingY = ((wallY*width)/2)-width+centerY;
    i < 19; i++, startingY -= width)
  {
    addWallDirection(startingX, startingY, startingZ, facingNorth, rows[i]);
  }
  int column1[] = {8,10, -1};
  int column2[] = {0,3,4,7, -1};
  int column3[] = {0,1,4,5,6,7,8,10,13,14, -1};
  int column4[] = {0,1,4,5,6,7,8,9,14, -1};
  int column5[] = {1,4,5,6,7,8,9,13,14, -1};
  int column6[] = {0,4,5,7,8,9,13, -1};
  int column7[] = {5,6,9,14, -1};
  int column8[] = {5,7,8,10,14, -1};
  int column9[] = {1,6,7,8,9,11,12,14, -1};
  int column10[] = {0,2,6,7,10,12,13, -1};
  int column11[] = {1,3,5,8,13,14, -1};
  int column12[] = {0,4,7,8,9,10,13,14, -1};
  int column13[] = {0,1,3,4,8,9,13,14, -1};
  int column14[] = {2,3,4,7,8,9,10,13,14, -1};
  int column15[] = {2,3,4,7,12,13,14, -1};
  int column16[] = {2,3,6,7,8,10,11,13,14, -1};
  int column17[] = {2,3,4,7,8,9,10,14, -1};
  int column18[] = {3,6,7,9,11,14, -1};
  int * columns[] = {column1, column2, column3, column4, column5, column6,
                     column7, column8, column9, column10, column11, column12,
                     column13, column14, column15, column16, column17, column18};
  for( i = 0,
    startingX = ((wallX*width)/-2)+width+centerX,
    startingY = ((wallY*width)/2)-(width*1.5f)+centerY;
    i < 18; i++, startingY -= width)
  {
    addWallDirection(startingX, startingY, startingZ, facingEast, columns[i]);
  }
}

v3_t Map::getRespawnPosition(std::size_t player_id)
{
  if(player_id > spawnPositions.size())
    return spawnPositions[0];
  return spawnPositions[player_id];
}

/*
 * Add walls from left to right. Assumes array ends with -1
 */
void Map::addWallDirection(float startingX, float startingY, float startingZ, v3_t dir, int values[])
{
  int width = 10;
  int height = 1; 
  int depth = 4;
  int x = 0;
  int j = 0;
  while(values[j] != -1)
  {
    if(values[j] == x){
      Wall* wall = new Wall(width, depth, height, v3_t(startingX,startingY, startingZ), dir, this);
      this->entities.push_back(wall);
      j++;
    }

    x++;
    startingX += width;
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

 std::vector<Projectile *> Map::getLiveProjectTile(){
   return liveProjectTile;
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
   liveProjectTile.push_back(ret);
   ret->reset();
   addToQtree(ret);
   return ret;
 }

 void Map::destroyProjectile(Projectile * proj)
 {
   if(proj->getOwner() == nullptr) //has already been removed
     return;

   if(proj->getOwner()->chargedProjectile == proj )
     proj->getOwner()->chargedProjectile = nullptr;

   proj->setOwner(NULL);
   freeProjectiles->push(proj);
   // should probably use a hasmap soon
   for(unsigned int i = 0; i < liveProjectTile.size(); i++) {
	   if(liveProjectTile.at(i) == proj) {
			liveProjectTile.erase(liveProjectTile.begin() + i);
	   }
   }
   removeFromQtree(proj);
 }

 //@mc 2box
void Map::separatePlayers(Player* player){
  std::vector<BoundingObj*> myObjs = player->getBoundingObjs();
  int restart = false;
  int restarts = 0;
  std::pair<bool,BoundingObj::vec3_t> res(false, BoundingObj::vec3_t(0,0,0));
  Entity * e = nullptr;
  
  for( unsigned int i=0; i < players.size(); i++ ){
    
    if(players[i] != player){ //if not the player try collide
      std::vector<BoundingObj*> objs = players[i]->getBoundingObjs();  
      for( auto boxes = objs.begin(); boxes != objs.end(); boxes++){

        for( auto myBoxes = myObjs.begin(); myBoxes != myObjs.end(); myBoxes++){
          std::pair<bool,BoundingObj::vec3_t> res2 = collide(*myBoxes,*boxes);
          if(res2.first){
            if(res2.second.magnitudesq() > res.second.magnitudesq()){
              e = (*boxes)->getEntity();
              res = res;
            }
          }
        }
        if(restart)
          break;

      }
      
      if(res.first){
        player->collidePlayer(std::pair<Entity*, BoundingObj::vec3_t>(e, res.second));
        restart = true;
      }
    }

    if(restart){
      restarts++;
      i = 0;
      restart = false;
    }
    if(restarts > 3)
      break;

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

void Map::initScores() {
  for (unsigned int i=0;i<ConfigManager::numPlayers();i++) {
    kills.push_back(0);
    wins.push_back(0);
  }
}

void Map::addEntity(Entity* e){
  entities.push_back(e);
  addToQtree(e);
}
