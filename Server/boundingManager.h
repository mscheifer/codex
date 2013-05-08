#pragma once
#include "boundingObj.h"
#include "boundingBox.h"
#include "boundingSphere.h"
#include "Ray.h"
#include <vector>

bool sortEntity(std::pair<Entity*,BoundingObj::vec3_t> a, std::pair<Entity*,BoundingObj::vec3_t> b);
std::pair<bool,BoundingObj::vec3_t> collide(const BoundingObj * a,const  BoundingObj * b);
std::pair<bool,BoundingObj::vec3_t> boxBox(const BoundingBox * a,const  BoundingBox * b);
std::pair<bool,BoundingObj::vec3_t> boxRay(const BoundingBox * a,const  Ray * b);
std::pair<bool,BoundingObj::vec3_t> sphereSphere(const BoundingSphere * a,const  BoundingSphere * b);
std::pair<bool,BoundingObj::vec3_t> sphereRay(const BoundingSphere * a,const  Ray* b);
void boxTest();
void sphereTest();
