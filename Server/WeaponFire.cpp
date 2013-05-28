#include "WeaponFire.h"
#include "Projectile.h"

WeaponFire::~WeaponFire(void)
{
}

WeaponFire::WeaponFire(v3_t c, Map* m, MAGIC_POWER basicAttack1)
  : Weapon(2.0, 300, c, m), basicAttack(basicAttack1) {

	Range_Cool_Down_Time = 500;
	Melee_Cool_Down_Time = 200;

  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(c.x,c.y,c.z),
  BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
  3,3,3);
  b->setEntity(this);
  //BoundingSphere* b = new BoundingSphere(gx::vector4(x,y,z),sphereRadius);
  boundingObjs.push_back(b);
}

void WeaponFire::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

Projectile* WeaponFire::attackRange(v3_t dir , v3_t pos, Player* owner)
{
	Projectile* pj = map->produceProjectile();
  //dir.normalize();
  //pj->setDirection(dir);
  //dir.scale(projectileSpeed); //TODO @alvin I took these out becasuse 
  pj->setVelocity(ZEROVEC);
  //pj->setPosition(pos);
  pj->setOwner(owner);
	//pj->setStrength(projectileStrength); it should be determined at the fire time
	//pj->setRange(100); //      based on the magic it is
  //pj->setFired(false); //defaults to false
  // pj->setChargeTime(1500);
  std::cout << "set proj m type " << basicAttack << std::endl;
  pj->setMagicType(basicAttack);

  Range_Cool_Down_Counter.restart();
	return pj;
}

WeaponType WeaponFire::getWeaponType() const{
  switch(basicAttack){
    case FIR1:
      return FIRE;
    case ICE1:
      return ICE;
    case THU1:
      return THUNDER;
    case B1:
      return BASIC;
  }
  return UNK;
}

float WeaponFire::getMpCost(){
  return ProjInfo[basicAttack].mpCost;
}