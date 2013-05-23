#pragma once
#include "boundingObj.h"
#include "boundingBox.h"
#include "boundingSphere.h"
#include "Ray.h"
#include <vector>

struct RayCollision{
  Entity * e;
  bool collided;
  BoundingObj::unit_t tfirst;
  BoundingObj::vec3_t normalAxis;
  BoundingObj::vec3_t parallelAxis;
  RayCollision() : e(nullptr), collided(false), tfirst(0), normalAxis(0,0,0), parallelAxis(0,0,0){}
};

//lesser first
bool sortEntity(std::pair<Entity*,BoundingObj::vec3_t> a, std::pair<Entity*,BoundingObj::vec3_t> b);
bool sortRayCollision(RayCollision a, RayCollision b);
//check for collisiosn
std::pair<bool,BoundingObj::vec3_t> collide(const BoundingObj * a,const  BoundingObj * b);
//check for collisiosn with ray
RayCollision rayCollide(const Ray* r, const  BoundingObj * b);

//helper collision methods
std::pair<bool,BoundingObj::vec3_t> boxBox(const BoundingBox * a,const  BoundingBox * b);
std::pair<bool,BoundingObj::vec3_t> boxRay(const BoundingBox * a,const  Ray * b);
std::pair<bool,BoundingObj::vec3_t> boxRay(const BoundingBox * a,const  Ray * b, 
  BoundingObj::unit_t & timeHit, BoundingObj::vec3_t& normAxis, BoundingObj::vec3_t& parallelAxis);
std::pair<bool,BoundingObj::vec3_t> sphereSphere(const BoundingSphere * a,const  BoundingSphere * b);
std::pair<bool,BoundingObj::vec3_t> sphereRay(const BoundingSphere * a,const  Ray* b);

//tester methods
void boxTest();
void sphereTest();
