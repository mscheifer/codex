#include "boundingSphere.h"
#include "Quadtree.h"

BoundingSphere::BoundingSphere():center(0,0,0), radius(0){
  updateRect();
}

BoundingSphere::BoundingSphere(float x, float y, float z, float radius) :
center(x,y,z)
{
  this->radius = radius;
  updateRect();
}

void BoundingSphere::move(const gx::vector3 & v){
  center = center+v;
  getQtree()->remove(*this);
  updateRect();
  getQtree()->insert(*this);
}

bool BoundingSphere::collideWith(const BoundingSphere & o){
  gx::vector3 v3 = gx::vector3(o.center.x-center.x, o.center.y - center.y, o.center.z - center.z);
  
  //v3 is direction vector and distance to separate is r1 + r2 - v3.mag
  if(v3.magnitude() < o.radius + radius){
    std::cout << "direction: x "<< v3.x << " y "<< v3.y << " z "<< v3.z << " \n dist " << (o.radius+radius)-v3.magnitude() << std::endl;
  }
  return v3.magnitude() < o.radius + radius;
}

bool BoundingSphere::collideWith(const Ray & r){
  //project the center onto the line of the ray
  float ab2 = r.getDirection().dot(r.getDirection());
  gx::vector3 ap = center - r.getOrigin();
  float ap_dot_ab = ap.dot(r.getDirection());
  float t = ap_dot_ab / ab2;

  if( t < 0.0f ) t = 0.0f;
  else if( t > 1.0f ) t = 1.0f;

  //q is closest point
  gx::vector4 q = r.getOrigin() + r.getDirection() * t;
  std::cout << "collide at point " << q.x << "," << q.y << "," << q.z << std::endl;
  
  //check if that point is in the circle
  gx::vector3 pq = q - center;
  float pq2 = pq.dot(pq);
  float r2 = radius * radius;
  return pq2 <= r2;
}

void BoundingSphere::updateRect(){
 getRect()->setCenter(center);
 getRect()->setHalfHeight(radius);
 getRect()->setHalfWidth(radius);
}

BoundingSphere::~BoundingSphere(void)
{
}
