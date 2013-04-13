#include "Map.h"





Map::Map(void)
{
	map_size = 15;
	entities = new Entity[10]; 
}


Map::~Map(void)
{
}

 Entity* Map::getEntities() {
	 return entities;
}


