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
class Weapon;
class PowerUp;

class Map
{
public:
  Map(void);
  ~Map(void);
  static const float Item_Pick_Up_Ranges;
  int map_size;
  std::vector<int> kills; //TODO make them private
  std::vector<int> wins;
  
  v3_t getRespawnPosition(std::size_t player_id);
  std::vector<Entity *> getEntity(void);
  std::vector<Player *> getPlayers(void);
  std::vector<Projectile *> getLiveProjectTile(void);
  Projectile* produceProjectile(void);
  void destroyProjectile(Projectile *);
  bool addPlayer(Player *);
  Quadtree* getQuadtreePtr(){ return &q; }
  void separatePlayers(Player* player);
  void addEntity(Entity* e);
  
  //add this entity to the quadtree, should only be called in a constructor
  //of an entity
  void addToQtree(Entity* e); 
  void removeFromQtree(Entity* e);
  void mapReset();
  void initScores();
private:
  std::vector<v3_t> spawnPositions;
	std::vector<Player *> players;
	std::vector<Entity *> entities;
	std::stack<Projectile *>* freeProjectiles;
	std::vector<Projectile *> liveProjectTile;
  Quadtree q;
  void initWalls(void);
  void initWallsOne(void);
  void initWallsTwo(void);
  void initPowerUps(void);
  void addWallDirection(float startingX, float startingY, float startingZ, v3_t dir, int values[]);
  
  void addWallChange(bool start, float startingX, float startingY, float startingZ, v3_t dir, int values[]);
};