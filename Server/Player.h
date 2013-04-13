#pragma once
#include <typeinfo>
#include <iostream>
#include "Entity.h"


class Player: public Entity
{
public:
  char name[20];
  Player(void);
  Player(int x, int y, int z);
  ~Player(void);
  bool attack(Entity *other);
  void moveForward();
  void moveBackward();
  void moveLeft();
  void moveRight();
  int getHealth();
  Coordinate const * getPosition(void);
private:
  int health;
  int maxHealth;
  int strength;
  int defense;
  Coordinate direction;
  const Coordinate* temp_coordinate;
  bool damage(Player *);
};