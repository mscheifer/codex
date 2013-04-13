#pragma once
#include <typeinfo>
#include <iostream>
#include "Entity.h"
#include "Physics.h"
#define MOVESCALE 100
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
  Coordinate getPosition();
private:
  int health;
  int maxHealth;
  int strength;
  int defense;
  bool damageBy(Player *);
  void fixPosition();
};