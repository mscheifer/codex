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

const int MOVESCALE = 3;

#define MAXWEAPONS 2
const int MAXJUMP = 2;
class Player: public Entity
{
public:
  static const float sphereRadius;
  static const Entity_Type type = PLAYER;
  v3_t acceleration;
  v3_t velocity;
  v3_t oldJumpVelocity;
  bool dead; //might be private. should be determined in handleAction
  bool minotaur; //might be private
  int player_id;
  char name[20];
  Player();
  Player(Position x, Position y, Position z, int assigned_id, Map *);
  ~Player(void);
  std::string getString();
  virtual bool attackBy(DeadlyEntity*);
  void update();
  void handleCollisions();  
  void updateBounds();  
  void updateBoundsSoft(); 
 
  bool moveTowardDirection(move_t degree, bool jump);
  void jump();
  void handleAction(ClientGameTimeAction a);
  
  float getHealth() { return health; }
  void setHealth(float);

  float getMana() {return mana;}
  void setMana(float);

  float getSpeed(){ return speed;}
  void setSpeed(float);

  Entity_Type getType() {
    return type;
  }

  void Player::serialize(sf::Packet& packet) const {
    packet << type; //not necessary
    Entity::serialize(packet);
    packet << this->player_id;
  }

  void deserialize(sf::Packet& packet) {
    uint32_t packetType; //not necessary
    packet >> packetType;
    Entity::deserialize(packet);
    packet >> this->player_id;
  }


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
  void init(Position x, Position y, Position z, int assigned_id, Map * m);
  void generateBounds(Position x,Position y,Position z);

  //helper functions for collisions
  bool collideWall(std::pair<Entity*,BoundingObj::vec3_t>& p);
  bool collidePlayer(std::pair<Entity*,BoundingObj::vec3_t>& p);
  bool collideProjectile(std::pair<Entity*,BoundingObj::vec3_t>& p);
};
