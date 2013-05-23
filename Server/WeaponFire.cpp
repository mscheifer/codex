#include "WeaponFire.h"
#include "Projectile.h"

WeaponFire::~WeaponFire(void)
{
}

WeaponFire::WeaponFire(v3_t c, Map* m):Weapon(2.0,300, c, 20.0, m) {
	Range_Cool_Down_Time = 500;
	Melee_Cool_Down_Time = 200;
	mpCost = 0; //TODO revert this value was 20 WTF this is already set in weapon @allen @alvin

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
  dir.normalize();
  pj->setDirection(dir);
  //dir.scale(projectileSpeed); //TODO @alvin I took these out becasuse 
  pj->setVelocity(ZEROVEC);
  pj->setPosition(pos);
  pj->setOwner(owner);
	//pj->setStrength(projectileStrength); it should be determined at the fire time
	pj->setRange(100); //      based on the magic it is
  pj->setFired(false);
  // pj->setChargeTime(1500);
  pj->setMagicType(FIRE1);

  Range_Cool_Down_Counter.restart();
	return pj;
}