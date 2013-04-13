#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
#include "Entity.h"
#include "Physics.h"
#include <string>
#include "ClientGameTimeAction.h"
#define MOVESCALE 100
class Player: public Entity
{
public:
  char name[20];
  Player(void);
  Player(int x, int y, int z);
  ~Player(void);
  virtual bool attackBy(Player*);
  void moveForward();
  void moveBackward();
  void moveLeft();
  void moveRight();
  void jump();
  int getHealth();
  Coordinate getPosition();
  std::string getString();
  void handleAction(ClientGameTimeAction a);
private:
  int health;
  int maxHealth;
  int strength;
  int defense;
  bool damageBy(Player *);
  void fixPosition();
};