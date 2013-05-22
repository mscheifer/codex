#include "boundingSphere.h"
#include "Quadtree.h"

//BoundingSphere::BoundingSphere():BoundingObj(gx::vector4(0,0,0)), radius(0){
//  updateRect();
//}
//BoundingSphere::BoundingSphere(gx::vector4 cen, float radius) :
//  BoundingObj(cen){
//    this->radius = radius;
//    updateRect();
//}

void BoundingSphere::updateRect(){
 getRect()->setCenter(center);
 getRect()->setHalfHeight(radius);
 getRect()->setHalfWidth(radius);
}

BoundingObj::vec3_t BoundingSphere::getMaxRadius( vec3_t axis ){
  axis.normalize();
  axis.scale( radius );
  return axis;
}

BoundingSphere::~BoundingSphere(void){}