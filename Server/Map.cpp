#include "Map.h"





Map::Map(void)
{
	map_size = 15;
	entities = (Entity**)calloc(Max_Entities,sizeof(Player));
}


Map::~Map(void)
{
}

 Entity** Map::getEntities() {
	 return (Entity**)(&entities[0]);
}


