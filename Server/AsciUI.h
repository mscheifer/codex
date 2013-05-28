#pragma once
#include <vector>

class Map;
class Player;
class Entity;

class AsciUI
{
public:
  AsciUI(void);
  ~AsciUI(void);
  int update(Map*);
  int render(Player*, std::vector<Entity *>);
};

