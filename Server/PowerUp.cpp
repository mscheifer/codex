#include "PowerUp.h"

const float PowerUp::powerUpWidth = 3.f;
const float PowerUp::powerUpHeight = 3.f;
const float PowerUp::powerUpDepth = 3.f;

PowerUp::PowerUp(v3_t p, Map* m, BUFF buff1): buffType(buff1)
{
  map = m;
  setPosition(p);
  setDirection(v3_t(0,1,0));
	Respown_Counter = sf::Clock();
	active = true;
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(0,0,0),BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
  powerUpWidth/2.f,powerUpHeight/2.f,powerUpDepth/2.f);
  b->setEntity(this);
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  boundingObjs.push_back(b);
  m->addToQtree(this);
}

void PowerUp::handleCollisions() {
  /*
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
      if(speedMutiplyer > 1 ) {
	      luckyGuy.setAttackSpeed(luckyGuy.getSpeed() + speedMutiplyer);
        luckyGuy.activateSpeedUp();
        luckyGuy.setSpeedUpTime(speedUpTime);
        luckyGuy.restartSpeedUpCounter();
      }
	    luckyGuy.setMana(luckyGuy.getMana() + manaMutiplyer);
      active = false;
      break;
    }
  }
	Respown_Counter.restart();
  */
}

void PowerUp::update() {
	if ((Respown_Counter.getElapsedTime().asMilliseconds() >= Respown_Time) && active == false) {
		active = true;
    map->addSpawnLocation(position);
    position = map->getRespawnPosition();
    map->addToQtree(this);
    render = true;
    buffType = static_cast<BUFF>(rand()%7+1);
	}
  //std::cout << " it's active? " << active << std::endl;
  if(active)
    updateBounds();
}

void PowerUp::pickUp(){
  if(!active)
    return;
  
  active = false;
  render = false;
  Respown_Counter.restart();
  map->removeFromQtree(this);
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

std::string PowerUp::toString(){
  std::stringstream ss;
  ss << position << direction << std::endl;
  return ss.str();
}