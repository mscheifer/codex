#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cmath>
#include "Entity.h"
#include "Physics.h"
#include <string>
#include "Weapon.h"
#include "WeaponFist.h"
#include "WeaponFire.h"
#include "ClientGameTimeAction.h"
#include "boundingManager.h"
#include "algorithm"


#define MOVESCALE 100
#define PI 3.1415926535897932384626433832795

#define MAXWEAPONS 2
const int MAXJUMP = 2;
class Player: public Entity
{
public:
  static const int sphereRadius = 33;

  bool dead; //might be private. should be determined in handleAction
  bool minotaur; //might be private
  int player_id;
  char name[20];
  Player(void);
  Player(Position x, Position y, Position z, int assigned_id);
  ~Player(void);
  std::string getString();
  virtual bool attackBy(DeadlyEntity*);
 
  bool moveTowardDirection(move_t degree);
  void jump();
  void handleAction(ClientGameTimeAction a);

  void onCollision(Entity* a);
  void updateBounds();
  
  float getHealth() { return health; }
  void setHealth(float);

  float getMana() {return mana;}
  void setMana(float);

  float getSpeed(){ return speed;}
  void setSpeed(float);



private:
  float health;
  float maxHealth;
  float mana;
  float maxMana;
  float defense;
  float speed;
  float castDownTime;
  sf::Clock castDownCounter;
  int jumpCount;
  bool canJump;
  bool attacking;
  Weapon* weapon[MAXWEAPONS]; //0 bare hand, 1 fireball
  int current_weapon_selection; //0 bare hand, 1 fireball
  bool damageBy(DeadlyEntity *);
  void fixPosition();
  void handleSelfAction(ClientGameTimeAction a);
  void handleOtherAction(ClientGameTimeAction a);
  void attack(ClientGameTimeAction a);
  bool isPlayer(void) {return true;};
  void init(Position x, Position y, Position z, int assigned_id);
};