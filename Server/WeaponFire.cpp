#include "WeaponFire.h"



WeaponFire::~WeaponFire(void)
{
}

WeaponFire::WeaponFire(v3_t c, Map* m):Weapon(2.0,300, c, 20.0, m) {
	Range_Cool_Down_Time = 500;
	Melee_Cool_Down_Time = 200;
	mpCost = 20; //TODO revert this value was 20

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