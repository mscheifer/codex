#include "Wall.h"


Wall::Wall(unsigned int w, unsigned int d, unsigned int h, Coordinate& sc):centerPositions()
{
  width = w;
  depth = d;
  height = h;
  centerPositions.push_back(sc);
  currentCenter = 0;
}


Wall::~Wall(void)
{
}

/**
  * Cycle through the possible positions of the wall
  */
void Wall::update()
{
  currentCenter++;
  if( currentCenter == centerPositions.size() )
    currentCenter = 0;
}

void Wall::addNewCenter(Coordinate & center)
{
  centerPositions.push_back(center);
}

void Wall::addNewCenters(std::vector<Coordinate>& centers)
{
  for(unsigned i = 0; i < centers.size(); i++)
    centerPositions.push_back(centers[i]);
}