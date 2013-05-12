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
  position += velocity * ConfigManager::serverTickLengthSec();
	
  /* TODO not sure what this does
  Coordinate nextPosition = ThreeDMovement(position2, direction2, 0);
	distanceLeftToTravel -= calculateDistanceInBetween(position2, nextPosition);	
	position2 = nextPosition;
	
	//if it already traveled a its range
	if(distanceLeftToTravel <= 0.0 ) {
		// destroy yourself and return
		map->destroyProjectile(this);
		return;
	}
  */

	updateBounds();
	// some collision detection

	if(/* colides with some entity*/ false) {
		std::vector<Entity> entities;
		for(unsigned int i = 0; i < entities.size() ; i++){
			if(entities[i].getType() == PLAYER) {
				// hits a player
				Player unLuckyPerson = *(Player*)&entities[i];
				unLuckyPerson.attackBy(this);
			}
		}
		map->destroyProjectile(this);
		//now destory youself
	}
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