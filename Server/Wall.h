#pragma once
#include "Entity.h"
#include <vector>
class Wall :
  public Entity
{
public:
  static const float sphereRadius;
  Wall(unsigned int width, unsigned int depth, unsigned int height, Coordinate startingCoordinate, Direction direct, Map*);
  ~Wall(void);
  void update(void);
  void addNewCenter(Coordinate);
  void addNewCenters(std::vector<Coordinate>&);
  void updateBounds();
private:
  // A vector of POSSIBLE positions
  std::vector<Coordinate> centerPositions;
  unsigned int currentCenter;
  unsigned int width;
  unsigned int depth;
  unsigned int height;
};

