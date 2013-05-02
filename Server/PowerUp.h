#pragma once
#include "Entity.h"

class PowerUp : Entity{
public:
  PowerUp() {}
  ~PowerUp() {}
  


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

protected:
  Coordinate position; // place it will respown
  Direction direction;
  Map* map;
  sf::Clock Respown_Counter;
  int Respown_Time;
};