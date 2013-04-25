#include "boundingSphere.h"

boundingSphere::boundingSphere(){
  boundingSphere(0,0,0,0);
}

boundingSphere::boundingSphere(float x, float y, float z, float radius) :
center(x,y,z)
{
  this->radius = radius;
}

void boundingSphere::move(const gx::vector3 & v){
  center = center+v;
}

bool boundingSphere::collideWith(boundingSphere & o){
  gx::vector3 v3 = gx::vector3(o.center.x-center.x, o.center.y - center.y, o.center.z - center.z);
  
  if(v3.magnitude() < o.radius + radius){
    std::cout << "direction: x "<< v3.x << " y "<< v3.y << " z "<< v3.z << " \n dist " << v3.magnitude()-(o.radius+radius) << std::endl;
  }
  return v3.magnitude() < o.radius + radius;
}

boundingSphere::~boundingSphere(void)
{
}
