#pragma once
#include "Entity.h"
#include "Player.h"
#include <stdlib.h>
class Map
{
public:
  Map(void);
  ~Map(void);
  static int const Max_Entities = 10;
  Entity** getEntities(void);
  int map_size;

private:
	Entity** entities;


};