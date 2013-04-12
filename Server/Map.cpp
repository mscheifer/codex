#include "Map.h"





Map::Map(void)
{
	map_size = 15;
	entities = (Entity**)malloc(sizeof(Player)*Max_Entities);
}


Map::~Map(void)
{
}

 Entity** Map::getEntities() {
	 return (Entity**)(&entities[0]);
}


