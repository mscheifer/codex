#include "PowerUp.h"

PowerUp::PowerUp(v3_t p, Map* m)
{
  map = m;
  setPosition(p);
	Respown_Counter = sf::Clock();
	active = true;
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(0,0,0),BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
  5,5,5);
  b->setEntity(this);
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  boundingObjs.push_back(b);
  m->addToQtree(this);
}

void PowerUp::handleCollisions() {
	//if it's not active, dont do anything

 
  if(active == false )
		return ;
    
  std::vector<std::pair<Entity*,BoundingObj::vec3_t>> entities =  detectCollision();

  for( auto it = entities.begin(); it != entities.end(); it++ ){
    Entity * e = it->first; 

    if(e->getType() == PLAYER) {
      // std::cout << "collided " << std::endl;
      Player luckyGuy = *(Player*)e;
      luckyGuy.setHealth(luckyGuy.getHealth() + healthMutiplyer);
	    luckyGuy.setSpeed(luckyGuy.getSpeed() + speedMutiplyer);
	    luckyGuy.setMana(luckyGuy.getMana() + manaMutiplyer);
    }
    
  }
	
	
	active = false;
	
	Respown_Counter.restart();
}

void PowerUp::update() {
	if ((Respown_Counter.getElapsedTime().asMilliseconds() >= Respown_Time) && active == false) {
		active = true;
	}
  updateBounds();
}

void PowerUp::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void PowerUp::serialize(sf::Packet& packet) const
{
  Entity::serialize(packet);
}

void PowerUp::deserialize(sf::Packet& packet)
{ 
  Entity::deserialize(packet);
}
