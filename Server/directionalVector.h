#pragma once
#include "SFML/Network/Packet.hpp"
#include <iostream>

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

  bool operator==(const Direction & other) const {
    if (this->x!=other.x) return false;
    if (this->y!=other.y) return false;
    if (this->z!=other.z) return false;
    return true;
  }
  
  bool operator!=(const Direction & other) const {
    return !(*this==other) ;
  }
  void serialize(sf::Packet & packet) const {
    packet << x;
    packet << y;
    packet << z;
  }

  void deserialize(sf::Packet & packet){
    packet >> x;
    packet >> y;
    packet >> z;
  }

  friend std::ostream& operator<<(std::ostream& os, const Direction& t)
  {
    os << t.x << ', ' << t.y << ', ' << t.z;
    return os;
  }
};
