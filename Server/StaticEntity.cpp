#include "StaticEntity.h"



StaticEntity::~StaticEntity(void)
{
}

StaticEntity::StaticEntity(float w, float d, float h, v3_t sc, v3_t direct, Map * m, Entity_Type e):static_entity_type(e)
{
  width = w;
  depth = d;
  height = h;
  centerPositions.push_back(sc);
  currentCenter = 0;
  position = sc;
  direction = direct;
  map = m;
  MoveClock = sf::Clock();
  MoveTime = 0;

  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(sc.x,sc.y,sc.z),
    BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
    w/2.f,h/2.f,d/2.f);
  b->rotate(direct,BoundingObj::vec3_t(0,0,1));
  b->setEntity(this);
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  boundingObjs.push_back(b);
  m->addToQtree(this);
}
  


/**
  * Cycle through the possible positions of the wall
  */
void StaticEntity::update()
{
  if(centerPositions.size() < 2)
    return;

  if(distanceLeft > 0)
  {
    v3_t distanceTravelled = velocity * ConfigManager::serverTickLengthSec();
	  position += distanceTravelled;
    distanceLeft -= distanceTravelled.magnitude();
    if(distanceLeft <=0) {
      distanceTravelled.normalize();
      distanceTravelled.scale(distanceLeft);
      position += distanceTravelled;
    }
    // Render only if the position is above ground
    render = (position.z > (-depth/2));
    this->updateBounds();
  } else if( MoveClock.getElapsedTime().asMilliseconds() < MoveTime ) { 
		  return;
  } else {
    MoveClock.restart();
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

void StaticEntity::addNewCenter(v3_t center)
{
  centerPositions.push_back(center);
}



void StaticEntity::serialize(sf::Packet& packet) const {
  Entity::serialize(packet);
  packet << scale;
  packet << static_cast<sf::Uint32>(static_entity_type);
}
void StaticEntity::deserialize(sf::Packet& packet) {
   Entity::deserialize(packet);
   packet >> scale;
   sf::Uint32 type;
   packet >> type;  
   static_entity_type = static_cast<Entity_Type>(type);

}
 

void StaticEntity::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void StaticEntity::updateBoundsSoft(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
}