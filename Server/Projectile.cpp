#include "Projectile.h"

Projectile::Projectile(Map* m)
{
	this->map = m;
  fired = false;
  charge_level = FIREONE;
  charging = true;
  charge_counter = sf::Clock();
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(0,0,0),BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
  1,1,1);
  b->setEntity(this);
  boundingObjs.push_back(b);
}

Projectile::~Projectile(void)
{
}

void Projectile::update(void) {

    if(charging && charge_level != FIRETHREE ) {
      if(charge_counter.getElapsedTime().asMilliseconds() > Charge_Time ) {
        charge_counter.restart();
        charge_level = (MAGIC_POWER)(charge_level  + 1);
        std::cout << "increase level heeeeer !" << std::endl;
      }
      return;
    }
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

float Projectile::getStrength() {
  return strength*magic_mutiplier[charge_level];
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
    if(e != owner &&  e->getType() != PROJECTILE) {
       map->destroyProjectile(this);
    }
  }

}

void Projectile::clearEvents(){
  fired = false;
}

void Projectile::fire(v3_t v) {
  velocity = v;
  fired = true;
  charging = false;
}

void Projectile::serialize(sf::Packet & packet) const {
  Entity::serialize(packet);
  packet << fired;
  //(*owner).serialize(packet);
} 
void Projectile::deserialize( sf::Packet & packet ) {
  Entity::deserialize(packet);
  packet >> fired;
  //delete owner; this segfaults
  //Player* owner = new Player();
  //(*owner).deserialize(packet);
 }