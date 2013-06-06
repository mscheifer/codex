#pragma once
#include <array>
#include <vector>
#include <stdlib.h>
#include <stack>
#include "Quadtree.h" //TODO remove comment
#include "Physics.h" //typedef
#include <stdlib.h> 

class StaticEntity;
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

  v3_t getRespawnPosition();
  const std::vector<Entity *>& getEntity(void);
  const std::vector<Player *>& getPlayers(void);
  std::vector<Projectile *> getLiveProjectTile(void);
  Projectile* produceProjectile(void);
  void destroyProjectile(Projectile *);
  bool addPlayer(Player *);
  Quadtree* getQuadtreePtr(){ return &q; }
  void separatePlayers(Player* player);
  void addEntity(Entity* e);
  void addSpawnLocation(v3_t);
  
  //add this entity to the quadtree, should only be called in a constructor
  //of an entity
  void addToQtree(Entity* e); 
  void removeFromQtree(Entity* e);
  void mapReset();
  void initScores();
  void assignName(std::string name, int id);
  std::vector<StaticEntity*> getStaticEntities(void);
private:
  std::vector<v3_t> spawnPositions;
	std::vector<Player *> players;
	std::vector<Entity *> entities;
	std::stack<Projectile *>* freeProjectiles;
	std::vector<Projectile *> liveProjectTile;
  std::vector<StaticEntity*> staticEntities;
  Quadtree q;
  void initSpawns(void);
  void initFloor(void);
  void initTestWalls(void);
  void initWalls(void);
  void initWallsRed(void);
  void initWallsBlue(void); 
  void initWallsOne(void);
  void initWallsTwo(void);
  void initStaticWalls(void);
  void initPowerUps(void);
  void initStaticEntities(void);
  void addWallDirection(float startingX, float startingY, float startingZ, v3_t dir, int values[]);
  void destHelper();
  void addWallChange(bool start, float startingX, float startingY, float startingZ, v3_t dir, int values[]);
};