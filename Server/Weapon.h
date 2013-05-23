#pragma once

#include <SFML/System/Clock.hpp>
#include "DeadlyEntity.h"

class Projectile;

class Weapon : public DeadlyEntity
{
public:
  static const Entity_Type type = WEAPON;
  static const WeaponType wtype = UNK;
  static const bool hasRangedAttack = false;
  static const bool hasMeleeAttack = false;
  Weapon() {/*TODO initialzie*/}
	Weapon(Map*);
	~Weapon(void);
	Weapon(float damage, float range, v3_t pos, float mpcost, Map*);

	bool canUseWeapon(bool range_attack, Player* owner);
	bool canPickUp() { return pickedUp; }
	virtual Projectile* attackMelee(v3_t dir, v3_t pos, Player* owner); 
  virtual Projectile* attackRange(v3_t dir, v3_t pos, Player* owner){return nullptr;}
  //pick up weapon, remove bounding box from map
  virtual bool pickUp();
  //add the bounding box again
  virtual bool dropDown(v3_t dropPosition);
  
  int getRange(void);
	int getDamage(void);
	float getMpCost(){ return mpCost; }
  virtual WeaponType getWeaponType() const{ return wtype; }
  virtual bool getHasRangedAttack() const { return hasRangedAttack; }
  virtual bool getHasMeleeAttack() const { return hasMeleeAttack; }
  
  void serialize(sf::Packet & packet) const;
  void deserialize(sf::Packet & packet); 
 
  Entity_Type getType() const { return type; }

protected:
	int Range_Cool_Down_Time; //cool down time between uses in milliseconds
	int Melee_Cool_Down_Time; 
	float projectileStrength;
  length_t projectileSpeed;
	length_t projectileRange; 
	float mpCost;
	bool pickedUp;
	sf::Clock Range_Cool_Down_Counter;
	sf::Clock Melee_Cool_Down_Counter;
};

