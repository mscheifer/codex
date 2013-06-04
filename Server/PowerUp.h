#pragma once
#include "Entity.h"
#include "Player.h"

class PowerUp : public Entity{
public:
  PowerUp(){};
  PowerUp(v3_t p, Map* m, BUFF ptype);
  
  static const float powerUpWidth;
  static const float powerUpHeight;
  static const float powerUpDepth;
  static const Entity_Type type = POWER_UP;
  void serialize(sf::Packet& packet) const;
  void setRespownTime(int r){ Respown_Time = r;}
  void deserialize(sf::Packet& packet);
  void handleCollisions();
  void update();
  void updateBounds(); //TODO this maybe should be in inherited (if differen sizes)
  //pick up weapon, remove bounding box from map
  void pickUp();

  Entity_Type getType() const { return type; }
  BUFF getBuffType() const { return buffType; }

protected:
  BUFF buffType;
  sf::Clock Respown_Counter;
  int Respown_Time;
	float healthMutiplyer;
	float manaMutiplyer;
  int speedUpTime; // this is in milli sec
	float speedMutiplyer;
	bool active;
};
