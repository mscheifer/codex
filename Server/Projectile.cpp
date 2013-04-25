#include "Projectile.h"


Projectile::Projectile(void)
{
}

Projectile::Projectile(Position r)
{
	range = r;
	distanceLeftToTravel = r;
}


Projectile::~Projectile(void)
{
}

void Projectile::update(void) {

	Coordinate nextPosition = ThreeDMovement(position, direction, 0);
	distanceLeftToTravel -= calculateDistanceInBetween(position, nextPosition);	
	
	//if it already traveled a its range
	if(distanceLeftToTravel <= 0.0 ) {
		// destroy yourself and return
		//Server.removeEntity(this);
		return;
	}
	// some collision detection

	if(/* colides with some entity*/ false) {
		std::vector<Entity> entities;
		for(int i = 0; i < entities.size() ; i++){
			if(entities[i].isPlayer()) {
				// hits a player
				Player unLuckyPerson = *(Player*)&entities[i];
				unLuckyPerson.attackBy(this);
			}
		}

		//now destory youself
		//Server.removeEntity(this);

	}
}
