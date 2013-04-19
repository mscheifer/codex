#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cmath>
#include "Entity.h"
#include "Physics.h"
#include <string>
#include "Weapon.h"
#include "ClientGameTimeAction.h"

#define MOVESCALE 100
#define PI 3.1415926535897932384626433832795

class Player: public Entity
{
public:
  int player_id;
  char name[20];
  Player(void);
  Player(Position x, Position y, Position z, int assigned_id);
  ~Player(void);
  virtual bool attackBy(DeadlyEntity*);
  bool moveTowardDirection(User_Movement degree);
  void jump();
  float getHealth() { return health; }
  std::string getString();
  void handleAction(ClientGameTimeAction a);
private:
  float health;
  float maxHealth;
  float defense;
  float speed;
  int jumpCount;
  Weapon weapon[2]; //0 bare hand, 1 fireball
  int current_weapon_selection; //0 bare hand, 1 fireball
  bool damageBy(DeadlyEntity *);
  void fixPosition();
  void handleSelfAction(ClientGameTimeAction a);
  void handleOtherAction(ClientGameTimeAction a);
  void attack(ClientGameTimeAction a);
};