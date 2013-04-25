#pragma once
#include "SFML/Network/Packet.hpp"

typedef float DirectionValue;
struct Direction
{
  DirectionValue x;
  DirectionValue y;
  DirectionValue z;
  Direction() :
    x(0), y(0), z(0){}
  Direction(DirectionValue a, DirectionValue b, DirectionValue c) :
    x(a), y(b), z(c){}

  void serialize(sf::Packet & packet){
    packet << x;
    packet << y;
    packet << z;
  }

  void deserialize(sf::Packet & packet){
    packet >> x;
    packet >> y;
    packet >> z;
  }
};
