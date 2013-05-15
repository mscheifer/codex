#pragma once
#include "Entity.h"
#include "Player.h"

class PowerUp : public Entity{
public:
  PowerUp() ;
  ~PowerUp() {}
  
  static const Entity_Type type = POWER_UP;


  void setPosition(Coordinate c) {
	  position = c;
  }
  void serialize(sf::Packet& packet) const
  {
    position.serialize(packet);
    direction.serialize(packet);
  }

  void deserialize(sf::Packet& packet)
  {
    position.deserialize(packet);
    direction.deserialize(packet);
  }

  void onCollision(Player*);
  void update();

protected:
  Coordinate position; // place it will respown
  Direction direction;
  Map* map;
  sf::Clock Respown_Counter;
  int Respown_Time;

private:
	float healthMutiplyer;
	float manaMutiplyer;
	float speedMutiplyer;
	bool active;
};