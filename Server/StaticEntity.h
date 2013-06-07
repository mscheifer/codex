#pragma once
#include "Entity.h"
#include <SFML/System.hpp>

class StaticEntity :
  public Entity
{
public:
  StaticEntity(){};
  StaticEntity(float w, float d, float h, v3_t sc, v3_t direct, Map * m, Entity_Type e);
  Entity_Type static_entity_type;
  int scale;
  void serialize(sf::Packet& packet) const;
  void deserialize(sf::Packet& packet);
  void addNewCenter(v3_t center);
  void updateBounds(void);
  void updateBoundsSoft(void);
  Entity_Type getType(void) const { return WALL;}
  void update(void);
  ~StaticEntity(void);
private:
  float width;
  float depth;
  float height;
  std::vector<v3_t> centerPositions;
  unsigned int currentCenter;
  float MoveTime;
  sf::Clock MoveClock;
  
  length_t distanceLeft;
 
};

