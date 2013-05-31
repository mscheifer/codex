#pragma once
#include <typeinfo>
#include <iostream>
#include <sstream>
//#include <cmath>
#include <algorithm>
#include <string>
#include "Entity.h"
#include "ClientGameTimeAction.h"

#include "Weapon.h"
const int MAXWEAPONS = 2;

class Player: public Entity
{
public:
  static length_t MOVESCALE();
  static length_t AIRMOVESCALE();
  static length_t JUMPSPEED();
  static int MAXJUMP();

  static const float playerWidth;
  static const float playerHeight;
  static const float playerDepth;
  static const Entity_Type type = PLAYER;
  bool dead; //might be private. should be determined in handleAction
  bool meleeAttack;
  bool weaponCall;
  bool collectPowerUp;
  WeaponType weaponCallType;
  WeaponType weapon1;
  WeaponType weapon2;
  int elapsedChargeTime;
  int totalChargeTime;
  MAGIC_POWER chargeMagicType;
  
  int player_id;
  std::string name;
  Projectile* chargedProjectile;
  int kills; //TODO private?
  int wins;
  Player();
  void reset(v3_t pos);
  Player(v3_t pos, int assigned_id, Map *);
  ~Player(void);
  std::string getString();
  virtual bool attackBy(Projectile*);
  void update();
  void handleCollisions();  
  void updateBounds();  
  void updateBoundsSoft();
  void clearEvents();
  bool collidePlayer(const std::pair<Entity*,BoundingObj::vec3_t>& p);
  
  bool moveTowardDirection(move_t degree, bool jump); //handle movement input WADS jump
  void handleAction(ClientGameTimeAction a);
  
  //getters and setters
  int getCurrentWeaponSelection() const { return current_weapon_selection; };
  float getHealth() const { return health; }
  void setHealth(float);
  float getMana() const {return mana;}
  void setMana(float);
  float getSpeed() const { return speed;}
  void setSpeed(float);
  void setSpeedUpTime(int s) { speedUpTime = s; };
  void activateSpeedUp(void) { speedUp = true ;};
  bool isSpeedUpActive(void) { return speedUp;};
  void setAttackSpeed(float s) { attackSpeed = s;};
  float getAttackSpeedDiv(void) {return attackSpeed;};
  void restartSpeedUpCounter(void) { speedUpCounter.restart();};
  WeaponType getPickupWeaponType() const{ return pickupWeaponType; }
  void setAsMinotaur(bool b);
  bool isMinotaur() const;
  Entity_Type getType() const { return type; }
  float getAttackCD() const;
  float getChargeCD() const;
  float getStrengthMultiplier() const;
  float getMovementMultiplier() const;
  float getManaRegenMultiplier() const;
  float getHealthRegenMultiplier() const;
  int getKills() const { return kills; }
  int getWins() const { return wins; }
  
  void serialize(sf::Packet& packet) const;
  void deserialize(sf::Packet& packet);
  const std::list<std::pair<BUFF,int>> & getBuffs() const { return buffs;}
  bool charging;
  bool walking;
  bool shotProjectile;
  bool attacked;
private:
  Weapon* pickup;
  WeaponType pickupWeaponType;
  std::list<std::pair<BUFF,int>> buffs;
  std::list<std::pair<BUFF,int>> inactiveBuffs;

  v3_t oldJumpVelocity; //the x,y velocity that should be applied
  bool minotaur; 
  float health;
  float healthRegen;
  float maxHealth;
  float mana;
  float manaRegen;
  float maxMana;
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

  bool damageBy(Projectile *);
  void handleSelfAction(ClientGameTimeAction a);
  void handleOtherAction(ClientGameTimeAction a);
  void attack(ClientGameTimeAction a);
  void init(v3_t pos, int assigned_id, Map * m);
  void generateBounds(v3_t pos);
  void restartJump(length_t zPosFix);
  bool correctMovementHit( Entity* e );
  void fireProjectile();
  v3_t getProjectilePosition(void);
  void die();
  void respawn(v3_t pos);
  void applyBuff( BUFF b);
  void addInactiveBuff( BUFF b, int time );
  std::pair<BUFF, int> getBuffReplacement( BUFF b );
  void updateBuffs();
  v3_t getFeetOrigin();

    //helper functions for collisions
  bool collideWall(const std::pair<Entity*,BoundingObj::vec3_t>& p);
  bool collideProjectile(const std::pair<Entity*,BoundingObj::vec3_t>& p);
  bool collidePowerUp(const std::pair<Entity*,BoundingObj::vec3_t>& p);
 
};
