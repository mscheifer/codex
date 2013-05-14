#pragma once
//#include "Entity.h"
//#include "Player.h"
//#include "Projectile.h"
#include <array>
#include <vector>
#include <stdlib.h>
#include <stack>
#include "Quadtree.h"
#include "Physics.h" //typedef

class Entity;
class Player;
class Projectile;
class Wall;

class Map
{
public:
  Map(void);
  ~Map(void);
  static const float Item_Pick_Up_Ranges;
  int map_size;
  
  std::vector<Entity *> getEntity(void);
  std::vector<Player *> getPlayers(void);
  Projectile* produceProjectile(void);
  void destroyProjectile(Projectile *);
  bool addPlayer(Player *);
  Quadtree* getQuadtreePtr(){ return &q; }
  void separatePlayers(Player* player);
  
  //add this entity to the quadtree, should only be called in a constructor
  //of an entity
  void addToQtree(Entity* e);
  void removeFromQtree(Entity* e);

private:
	std::vector<Player *> players;
	std::vector<Entity *> entities;
	std::stack<Projectile *>* freeProjectiles;
	std::vector<Projectile *> liveProjectTile;
  Quadtree q;
  void initWalls(void);
  void addWallDirection(int startingX, int startingY, int startingZ, v3_t dir, int values[]);
};