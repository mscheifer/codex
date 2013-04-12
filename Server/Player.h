#pragma once
#include "3DCoordinate.h"
class Player
{
public:
  Player(void);
  ~Player(void);
  bool attack(Player);

private:
  int health;
  int maxHealth;
  int defense;
  Coordinate position;
  Coordinate direction;
};

