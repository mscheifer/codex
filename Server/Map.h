#pragma once
//#include "Entity.h"
//#include "Player.h"
//#include "Projectile.h"
#include <array>
#include <vector>
#include <stdlib.h>
#include <stack>
#include "Quadtree.h"

class Entity;
class Player;
class Projectile;

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

private:
	std::vector<Player *> players;
	std::vector<Entity *> entities;
	std::stack<Projectile *> freeProjectiles;
	std::vector<Projectile *> liveProjectTile;
  Quadtree q;
  void addToQtree(Entity* e);
  void removeFromQtree(Entity* e);

};