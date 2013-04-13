#include "Map.h"





Map::Map(void)
{
	map_size = 15;
	
}


Map::~Map(void)
{
}

std::array<Entity, Map::Max_Entities> Map::getEntities() {
	 return entities;
}
  
 std::array<Player, Map::Number_Of_Players> Map::getPlayers(){
	 return players;
 }


