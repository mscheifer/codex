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
  static const float Item_Pick_Up_Ranges;
  int map_size;
  
  std::array<Entity, Max_Entities > getEntities(void);
  std::array<Player, Number_Of_Players> getPlayers(void);

private:
	std::array<Player, Map::Number_Of_Players> players;
	std::array<Entity, Map::Max_Entities> entities;
	

};