#include "Projectile.h"

Projectile::Projectile(Map* m)
{
	this->map = m;

  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(0,0,0),BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
  5,5,5);
  b->setEntity(this);
  boundingObjs.push_back(b);
}

Projectile::~Projectile(void)
{
}

void Projectile::update(void) {
  v3_t distanceTravelled = velocity * ConfigManager::serverTickLengthSec();
	position += distanceTravelled;
  
  //see if travelled full range
  distanceLeftToTravel -= distanceTravelled.magnitude();
  if(distanceLeftToTravel <= 0.0){
    map->destroyProjectile(this);
    return;
  }

	updateBounds();
	// some collision detection
}

void Projectile::setOwner(Player * player){
  owner = player;
}

void Projectile::setStrength(float f) {
	strength = f;
}

void Projectile::setRange(length_t r) {
	range = r;
	distanceLeftToTravel = r;
}

void Projectile::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Projectile::updateBoundsSoft(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Projectile::handleCollisions() {
  
  std::vector<std::pair<Entity*,BoundingObj::vec3_t>> entities =  detectCollision();

  for( auto it = entities.begin(); it != entities.end(); it++ ){
    Entity * e = it->first; 
    if(e != owner) {
       map->destroyProjectile(this);
    }
  }

}