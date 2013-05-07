#include "Projectile.h"




Projectile::Projectile( Map* m)
{

	this->map = m;
}


Projectile::~Projectile(void)
{
}

void Projectile::update(void) {

	Coordinate nextPosition = ThreeDMovement(position, direction, 0);
	distanceLeftToTravel -= calculateDistanceInBetween(position, nextPosition);	
	position = nextPosition;
	
	//if it already traveled a its range
	if(distanceLeftToTravel <= 0.0 ) {
		// destroy yourself and return
		map->destroyProjectile(this);
		return;
	}

	updateBoundsOnTree();
	// some collision detection

	if(/* colides with some entity*/ false) {
		std::vector<Entity> entities;
		for(unsigned int i = 0; i < entities.size() ; i++){
			if(entities[i].isPlayer()) {
				// hits a player
				Player unLuckyPerson = *(Player*)&entities[i];
				unLuckyPerson.attackBy(this);
			}
		}
		map->destroyProjectile(this);
		//now destory youself
	}
}

void Projectile::setOwner(Player * player)
{
  owner = player;
}

void Projectile::setStrength(float f) {
	strength = f;
}

void Projectile::setRange(Position r) {

	range = r;
	distanceLeftToTravel = r;
}