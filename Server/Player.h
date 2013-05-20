#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <string>
#include "Entity.h"
#include "ClientGameTimeAction.h"

#include "PowerUp.h"
#include "Projectile.h"
#include "Weapon.h"
#include "WeaponFist.h"
#include "WeaponFire.h"

#define MAXWEAPONS 2

class Player: public Entity
{
public:
  static const length_t MOVESCALE;// = 2;//ConfigManager::playerMovescale();
  static const length_t AIRMOVESCALE;// = 2;//ConfigManager::playerAirMovescale();
  static const length_t JUMPSPEED;// = 2;//ConfigManager::playerJumpSpeed();
  static const int MAXJUMP;// = 2;//ConfigManager::playerMaxJump();

  static const float playerWidth;
  static const float playerHeight;
  static const float playerDepth;
  static const Entity_Type type = PLAYER;
  bool dead; //might be private. should be determined in handleAction
  bool minotaur; //might be private
  int player_id;
  std::string name;

  Player();
  Player(v3_t pos, int assigned_id, Map *);
  ~Player(void);
  std::string getString();
  virtual bool attackBy(DeadlyEntity*);
  void update();
  void handleCollisions();  
  void updateBounds();  
  void updateBoundsSoft();
  //void clearEvents();
  
  //helper functions for collisions
  bool collideWall(const std::pair<Entity*,BoundingObj::vec3_t>& p);
  bool collidePlayer(const std::pair<Entity*,BoundingObj::vec3_t>& p);
  bool collideProjectile(const std::pair<Entity*,BoundingObj::vec3_t>& p);
 
  bool moveTowardDirection(move_t degree, bool jump); //handle movement input WADS jump
  void handleAction(ClientGameTimeAction a);
  
  //getters and setters
  float getHealth() { return health; }
  void setHealth(float);
  float getMana() {return mana;}
  void setMana(float);
  float getSpeed(){ return speed;}
  void setSpeed(float);
  WeaponType getPickupWeaponType() const{ return pickupWeaponType; }

  Entity_Type getType() const {
    return type;
  }
  
  void serialize(sf::Packet& packet) const;
  void deserialize(sf::Packet& packet);
  

private:
  Weapon* pickup;
  WeaponType pickupWeaponType;

  v3_t oldJumpVelocity; //the x,y velocity that should be applied
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
  void handleSelfAction(ClientGameTimeAction a);
  void handleOtherAction(ClientGameTimeAction a);
  void attack(ClientGameTimeAction a);
  void init(v3_t pos, int assigned_id, Map * m);
  void generateBounds(v3_t pos);
  void restartJump(length_t zPosFix);
  v3_t correctMovement(v3_t movementDirection, bool slide);
};
