#pragma once
#include "Entity.h"
#include <vector>
class Wall : public Entity {
public:
  static const float sphereRadius;
  static const Entity_Type type = WALL;
  Wall ();
  Wall(unsigned int width, unsigned int depth, unsigned int height, v3_t startingCoordinate, v3_t direct, Map*);
  ~Wall(void);
  void update(void);
  void addNewCenter(v3_t);
  void addNewCenters(std::vector<v3_t>&);
  void updateBounds();
  void updateBoundsSoft(); 
  void serialize(sf::Packet & packet ) const {
    packet << type; 
    Entity::serialize(packet);
    packet << static_cast<sf::Uint32>(centerPositions.size());
    for (auto it=centerPositions.begin(); it!=centerPositions.end(); it++)
      (*it).serialize(packet);
    packet << currentCenter;
    packet << width;
    packet << depth;
    packet << height;
  } 
  void deserialize(sf::Packet & packet ) {
    Entity::deserialize(packet);
    sf::Uint32 size = 0;
    centerPositions.clear();
    packet >> size;
    for (auto i = 0u; i < size; i++) {
       v3_t v;
       v.deserialize(packet);
       centerPositions.push_back(v);
    }
    packet >> currentCenter;
    packet >> width;
    packet >> depth;
    packet >> height;
  }
  Entity_Type getType() {
    return type;
  }
private:
  // A vector of POSSIBLE positions
  std::vector<v3_t> centerPositions;
  unsigned int currentCenter;
  unsigned int width;
  unsigned int depth;
  unsigned int height;
};

