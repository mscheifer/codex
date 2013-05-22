#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <string>
#include "Entity.h"
#include "ClientGameTimeAction.h"

#include "Weapon.h"
#define MAXWEAPONS 2

class Player: public Entity
{
public:
  static const length_t MOVESCALE;
  static const length_t AIRMOVESCALE;
  static const length_t JUMPSPEED;
  static const int MAXJUMP;

  static const float playerWidth;
  static const float playerHeight;
  static const float playerDepth;
  static const Entity_Type type = PLAYER;
  bool dead; //might be private. should be determined in handleAction
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
  void setSpeedUpTime(int s) { speedUpTime = s; };
  void activateSpeedUp(void) { speedUp = true ;};
  bool isSpeedUpActive(void) { return speedUp;};
  void setAttackSpeed(float s) { attackSpeed = s;};
  float getAttackSpeedDiv(void) {return attackSpeed;};
  void restartSpeedUpCounter(void) { speedUpCounter.restart();};
  WeaponType getPickupWeaponType() const{ return pickupWeaponType; }
  void setAsMinotaur(bool b);
  bool isMinotaur();
  Entity_Type getType() const { return type; }
  
  void serialize(sf::Packet& packet) const;
  void deserialize(sf::Packet& packet);

private:
  Weapon* pickup;
  WeaponType pickupWeaponType;

  v3_t oldJumpVelocity; //the x,y velocity that should be applied
  bool minotaur; //might be private
  float health;
  float healthRegen;
  float maxHealth;
  float mana;
  float manaRegen;
  float maxMana;
  Projectile* chargedProjectile;
  float defense;
  float speed;
  int speedUpTime;
  float attackSpeed;
  float castDownTime;
  sf::Clock castDownCounter;
  sf::Clock speedUpCounter;
  bool speedUp;
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
  bool correctMovementHit( Entity* e );
  v3_t getProjectilePosition(void);
};
