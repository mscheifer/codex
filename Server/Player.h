#pragma once
#include <typeinfo>
#include <iostream>
#include "Entity.h"

class Player: public Entity
{
public:
  Player(void);
  Player(int x, int y, int z);
  ~Player(void);
  virtual bool attackBy(Player*);
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
  bool damageBy(Player *);
};