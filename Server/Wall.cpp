#include "Wall.h"

const float Wall::sphereRadius = 5.0f;

Wall::Wall(unsigned int w, unsigned int d, unsigned int h, v3_t sc, v3_t direct, Map * m):centerPositions()
{
  width = w;
  depth = d;
  height = h;
  centerPositions.push_back(sc);
  currentCenter = 0;
  position = sc;
  direction = direct;
  map = m;

  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(direct.x,direct.y,direct.z),
    BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
    w/2.2,h/2.2,d/2.2);
  b->setEntity(this);
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  boundingObjs.push_back(b);
  m->addToQtree(this);
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

void Wall::addNewCenter(v3_t center)
{
  centerPositions.push_back(center);
}

void Wall::addNewCenters(std::vector<v3_t>& centers)
{
  for(unsigned i = 0; i < centers.size(); i++)
    centerPositions.push_back(centers[i]);
}

void Wall::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Wall::updateBoundsSoft(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
}