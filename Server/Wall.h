#pragma once
#include "Entity.h"
#include <vector>
class Wall :
  public Entity
{
public:
  static const float sphereRadius;
  static const Entity_Type type = WALL;
  Wall(unsigned int width, unsigned int depth, unsigned int height, Coordinate startingCoordinate, Direction direct, Map*);
  ~Wall(void);
  void update(void);
  void addNewCenter(Coordinate);
  void addNewCenters(std::vector<Coordinate>&);
  void updateBounds();
  void updateBoundsSoft(); 
  Entity_Type getType() {
    return type;
  }
private:
  // A vector of POSSIBLE positions
  std::vector<Coordinate> centerPositions;
  unsigned int currentCenter;
  unsigned int width;
  unsigned int depth;
  unsigned int height;
};

