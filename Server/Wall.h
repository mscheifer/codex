#pragma once
#include <vector>
#include <SFML/System.hpp>
#include "Entity.h"

class Wall : public Entity {
public:
  static const float sphereRadius;
  static const Entity_Type type = WALL;

  Wall ();
  Wall(float width, float depth, float height, v3_t startingCoordinate, v3_t direct, Map*);
  ~Wall(void);
  void update(void);
  void addNewCenter(v3_t);
  void addNewCenters(std::vector<v3_t>&);
  void setWallChangeTime(float t);
  void updateBounds();
  void updateBoundsSoft(); 
  void serialize(sf::Packet & packet ) const;
  void deserialize(sf::Packet & packet );

  Entity_Type getType() const { return type; }
private:
  // A vector of POSSIBLE positions
  // why? --matt
  std::vector<v3_t> centerPositions;
  float wallMoveTime;
  sf::Clock wallMoveClock;
  unsigned int currentCenter;
  float width;
  float depth;
  float height;
  length_t distanceLeft;
};

