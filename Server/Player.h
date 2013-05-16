#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <string>
#include "Entity.h"
#include "Physics.h"
#include "Weapon.h"
#include "WeaponFist.h"
#include "WeaponFire.h"
#include "ClientGameTimeAction.h"
#include "boundingManager.h"
#include "PowerUp.h"

const int MOVESCALE = 3;
const length_t AIRMOVESCALE = 1;
const length_t JUMPSPEED = 20;
const int MAXJUMP = 5;

#define MAXWEAPONS 2

class Player: public Entity
{
public:
  static const float sphereRadius;
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

  void serialize(sf::Packet& packet) const {
    Entity::serialize(packet);
    packet << this->player_id;
    //acceleration.serialize(packet);
    //velocity.serialize(packet);
    //oldJumpVelocity.serialize(packet);
    packet << dead; 
    packet << minotaur; //might be private
    packet << name;
    packet << health;
    packet << maxHealth;
    packet << mana;
    packet << maxMana;
    packet << defense;
    packet << speed;
    packet << castDownTime; //not needed on client ?
    //sf::Clock castDownCounter;
    packet << jumpCount; // not needed on client ?
    packet << canJump; //not needed on client ?
    packet << attacking;  //not neede on client ?
    //Weapon* weapon[MAXWEAPONS]; 
    // change the array to vector ?
    packet << static_cast<sf::Uint32>(pickupWeaponType);
    packet << current_weapon_selection; 
  }

  void deserialize(sf::Packet& packet) {
    Entity::deserialize(packet);
    packet >> this->player_id;
    //acceleration.deserialize(packet);
    //velocity.deserialize(packet);
    //oldJumpVelocity.deserialize(packet);
    packet >>dead; 
    packet >>minotaur; //might be private
    packet >> name;
    packet >> health;
    packet >> maxHealth;
    packet >> mana;
    packet >> maxMana;
    packet >> defense;
    packet >> speed;
    packet >> castDownTime; //not needed on client ?
    //sf::Clock castDownCounter;
    packet >> jumpCount; // not needed on client ?
    packet >> canJump; //not needed on client ?
    packet >> attacking;  //not neede on client ?
    //Weapon* weapon[MAXWEAPONS]; 
    // change the array to vector ?
    sf::Uint32 pickupWeaponTypeUint32;
    packet >> pickupWeaponTypeUint32;
    pickupWeaponType = static_cast<WeaponType>(pickupWeaponTypeUint32);
    packet >> current_weapon_selection; 
  }

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
};
