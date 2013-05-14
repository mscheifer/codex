#pragma once

#include <SFML/System/Clock.hpp>
#include "DeadlyEntity.h"

class Projectile;

class Weapon : public DeadlyEntity
{
public:
  static const Entity_Type type = WEAPON;
  Weapon() {/*TODO initialzie*/}
	Weapon(Map*);
	~Weapon(void);
	Weapon(float damage, float range, v3_t pos, float mpcost, Map*);
	int getRange(void);
	int getDamage(void);
	length_t projectileSpeed;
	float getMpCost(){ return mpCost; }
	void handleAction(ClientGameTimeAction);
	void onCollision(Entity*);
	bool canUseWeapon(bool range_attack);
	void useWeapon(bool range_attack);
	bool canPickUp() { return pickedUp; }
	virtual bool attackMelee(); 
	virtual Projectile* attackRange(v3_t dir, v3_t pos);
  virtual bool pickUp(){ return false; };
  virtual bool dropDown(v3_t dropPosition){ position = dropPosition; return false; };
  void serialize(sf::Packet & packet) const {
	  packet << type;
    Entity::serialize(packet);
    //Range_Cool_Down_Time; 
    //Melee_Cool_Down_Time; 
    //float projectileStrength;
    //length_t projectileRange; 
    packet << mpCost;
    packet << pickedUp;
    //sf::Clock Range_Cool_Down_Counter;
    //sf::Clock Melee_Cool_Down_Counter;
  }
  void deserialize(sf::Packet & packet) {
	  Entity::deserialize(packet);
    //int Range_Cool_Down_Time; 
    //int Melee_Cool_Down_Time; 
    //float projectileStrength;
    //length_t projectileRange; 
    packet >> mpCost;
    packet >> pickedUp;
    //sf::Clock Range_Cool_Down_Counter;
    //sf::Clock Melee_Cool_Down_Counter;
  }
  Entity_Type getType() {
    return type;
  }

protected:
	int Range_Cool_Down_Time; //cool down time between uses in milliseconds
	int Melee_Cool_Down_Time; 
	float projectileStrength;
	length_t projectileRange; //TODO @alvin @allen should this be here?, projectile has its own range
	float mpCost;
	bool pickedUp;
	sf::Clock Range_Cool_Down_Counter;
	sf::Clock Melee_Cool_Down_Counter;
};

