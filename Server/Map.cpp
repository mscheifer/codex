#include "Map.h"





Map::Map(void)
{
	map_size = 25;
}


Map::~Map(void)
{
}

 Entity** Map::getEntities() {
	 return entities;
}


