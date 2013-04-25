#pragma once
#include "Entity.h"
#include "Player.h"
#include "Projectile.h"
#include <array>
#include <vector>
#include <stdlib.h>
#include <stack>


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
private:
	std::vector<Player *> players;
	std::vector<Entity *> entities;
	std::stack<Projectile *> freeProjectiles;
	std::vector<Projectile *> liveProjectTile;

};