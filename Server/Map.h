#pragma once
#include "Entity.h"
#include "Player.h"
#include <array>
#include <stdlib.h>
class Map
{
public:
  Map(void);
  ~Map(void);
  static int const Max_Entities = 10;
  static int const Number_Of_Players = 4;
  int map_size;
  
  Player players[Number_Of_Players];

  Entity* getEntities(void);
  std::array<Player, Number_Of_Players> getPlayers(void);

private:
	Entity* entities;


};