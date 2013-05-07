#include "Wall.h"

const float Wall::sphereRadius = 5.0f;

Wall::Wall(unsigned int w, unsigned int d, unsigned int h, Coordinate& sc, Direction direct, Map * m):centerPositions()
{
  width = w;
  depth = d;
  height = h;
  centerPositions.push_back(sc);
  currentCenter = 0;
  position = sc;
  direction = direct;

  BoundingBox* b = new BoundingBox(gx::vector4(direct.x,direct.y,direct.z),gx::vector3(1,0,0),gx::vector3(0,1,0),gx::vector3(0,0,1),
    sphereRadius,sphereRadius,sphereRadius);
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  boundingObjs.push_back(b);
  m->addToQtree(this);
  this->map = m;
  updateBounds();
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
  position = centerPositions[currentCenter];
}

void Wall::addNewCenter(Coordinate center)
{
  centerPositions.push_back(center);
}

void Wall::addNewCenters(std::vector<Coordinate>& centers)
{
  for(unsigned i = 0; i < centers.size(); i++)
    centerPositions.push_back(centers[i]);
}

void Wall::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenter(gx::vector4(position.x, position.y, position.z));
}