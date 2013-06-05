#pragma once
#include "Entity.h"

class StaticEntity :
  public Entity
{
public:
  StaticEntity(){};
  StaticEntity(Entity_Type e) : static_entity_type(e) {};
  Entity_Type static_entity_type;
  int scale;
  void serialize(sf::Packet& packet) const;
  void deserialize(sf::Packet& packet);
 
  ~StaticEntity(void);
};

