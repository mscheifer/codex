#include "Wall.h"

const float Wall::sphereRadius = 5.0f;

Wall::Wall() {
  //TODO initialize
}
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
  wallMoveClock = sf::Clock();
  wallMoveTime = 0;

  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(sc.x,sc.y,sc.z),
    BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
    w/2.f,h/2.f,d/2.f);
  b->rotate(direct,BoundingObj::vec3_t(0,0,1));
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
  if(centerPositions.size() < 2)
    return;

  if(distanceLeft > 0)
  {
    v3_t distanceTravelled = velocity * ConfigManager::serverTickLengthSec();
	  position += distanceTravelled;
    distanceLeft -= distanceTravelled.magnitude();
    this->updateBounds();
  } else if( wallMoveClock.getElapsedTime().asMilliseconds() < wallMoveTime ) {
		  return;
  } else {
    wallMoveClock.restart();
    currentCenter++;
    if( currentCenter == centerPositions.size() )
      currentCenter = 0;
    v3_t temp = centerPositions[currentCenter]; 
    v3_t targetVector = v3_t(temp.x - position.x, temp.y - position.y, temp.z - position.z);
    distanceLeft = targetVector.magnitude();
    targetVector.normalize();
    targetVector.scale(5);
    velocity = targetVector;
    this->updateBounds();
  }
}

void Wall::addNewCenter(v3_t center)
{
  centerPositions.push_back(center);
}

void Wall::addNewCenters(std::vector<v3_t>& centers)
{
  for(unsigned i = 0; i < centers.size(); i++)
    this->addNewCenter(centers[i]);
}

void Wall::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Wall::updateBoundsSoft(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Wall::setWallChangeTime(float t)
{
  wallMoveTime = t;
}

  void Wall::serialize(sf::Packet & packet ) const {
    Entity::serialize(packet);
    /*
    packet << static_cast<sf::Uint32>(centerPositions.size());
    for (auto it=centerPositions.begin(); it!=centerPositions.end(); it++)
      (*it).serialize(packet);
    packet << currentCenter;
    packet << width;
    packet << depth;
    packet << height;
    */
  } 
  void Wall::deserialize(sf::Packet & packet ) {
    Entity::deserialize(packet);
    /*
    sf::Uint32 size;
    packet >> size;
    centerPositions.clear();
    for (auto i = 0u; i < size; i++) {
       v3_t v;
       v.deserialize(packet);
       centerPositions.push_back(v);
    }
    packet >> currentCenter;
    packet >> width;
    packet >> depth;
    packet >> height;
    */
  }