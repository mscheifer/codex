#pragma once
#include "Entity.h"

class Map
{
public:
  Map(void);
  ~Map(void);
  Entity** getEntities(void);
  int map_size;

private:
	Entity* (entities[10]);


};