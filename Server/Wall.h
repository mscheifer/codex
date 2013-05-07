#pragma once
#include "entity.h"
#include <vector>
class Wall :
  public Entity
{
public:
  Wall(unsigned int width, unsigned int depth, unsigned int height, Coordinate& startingCoordinate);
  ~Wall(void);
  void update(void);
  void addNewCenter(Coordinate &);
  void addNewCenters(std::vector<Coordinate>&);
private:
  // A vector of POSSIBLE positions
  std::vector<Coordinate> centerPositions;
  unsigned int currentCenter;
  unsigned int width;
  unsigned int depth;
  unsigned int height;
};

