#include "Weapon.h"
#include "Projectile.h"

Weapon::Weapon(Map* m)
{
	Range_Cool_Down_Time = 0;
	Melee_Cool_Down_Time = 0;
	mpCost = 0;
	Range_Cool_Down_Counter = sf::Clock();
	Melee_Cool_Down_Counter = sf::Clock();
	this->map = m;
}

Weapon::~Weapon()
{
}

Weapon::Weapon(float damage, float ran, v3_t pos, Map* m)
{
	Range_Cool_Down_Time = 0;
	Melee_Cool_Down_Time = 0;
	Range_Cool_Down_Counter = sf::Clock();
	Melee_Cool_Down_Counter = sf::Clock();
	strength = damage;
	range = ran;
	position = pos;
  direction = v3_t(0,0,1);
	projectileSpeed = 100.0; // TODO pending removal
	projectileRange = 300; //pending removal
	projectileStrength = 26; //pending removal
	this->map = m;
}

bool Weapon::canUseWeapon(bool range_attack, Player* Owner) {
  int range_counter = Range_Cool_Down_Counter.getElapsedTime().asMilliseconds();
  int melee_counter = Melee_Cool_Down_Counter.getElapsedTime().asMilliseconds();
  
  float cdMult = Owner->getAttackCD();

	if(		( getHasRangedAttack() && range_attack &&  range_counter > Range_Cool_Down_Time*cdMult)
		||	( getHasMeleeAttack() && !range_attack &&  melee_counter > Melee_Cool_Down_Time*cdMult)){
			return true;
	}
	return false;
}

Projectile* Weapon::attackMelee(v3_t dir , v3_t pos, Player* owner)
{
	Projectile* pj = map->produceProjectile();
  dir.normalize();
  pj->setDirection(dir);
  dir.scale(projectileSpeed);
  pj->setVelocity(dir);
  pj->setPosition(pos);
  pj->setOwner(owner);
	pj->setStrength(projectileStrength);
	pj->setRange(1);
  //pj->setFired(true); //TODO add a sound event for melee
  Melee_Cool_Down_Counter.restart();
	return pj;
}

bool Weapon::pickUp(){
  if(pickedUp)
    return false;
  
  render = false;
  map->removeFromQtree(this);
  return true;
}

bool Weapon::dropDown(v3_t dropPosition){
  //if(!pickedUp)
  //  return false;

  render = true;
  position = dropPosition;
  pickedUp = false;
  map->addToQtree(this);
  return true;
} 
void Weapon::serialize(sf::Packet & packet) const {
    Entity::serialize(packet);
    //Range_Cool_Down_Time; 
    //Melee_Cool_Down_Time; 
    //float projectileStrength;
    //length_t projectileRange; 
    packet << mpCost;
    packet << pickedUp;
    //sf::Clock Range_Cool_Down_Counter;
    //sf::Clock Melee_Cool_Down_Counter;
  }
  void Weapon::deserialize(sf::Packet & packet) {
	  Entity::deserialize(packet);
    //int Range_Cool_Down_Time; 
    //int Melee_Cool_Down_Time; 
    //float projectileStrength;
    //length_t projectileRange; 
    packet >> mpCost;
    packet >> pickedUp;
    //sf::Clock Range_Cool_Down_Counter;
    //sf::Clock Melee_Cool_Down_Counter;
  }


//
//void Weapon::useWeapon( bool range_attack){
//	if(range_attack) {
//		Range_Cool_Down_Counter.restart();
//	} else {
//		Melee_Cool_Down_Counter.restart();
//	}
//}
//void Weapon::handleAction(ClientGameTimeAction){
//
//}
//
//void Weapon::onCollision(Entity*) {
//
//}
