#pragma once
#include "boundingObj.h"
#include "boundingBox.h"
#include "boundingSphere.h"
#include "Ray.h"
#include <vector>

bool sortEntity(std::pair<Entity*,gx::vector3> a, std::pair<Entity*,gx::vector3> b);
std::pair<bool,gx::vector3> collide(const BoundingObj * a,const  BoundingObj * b);
std::pair<bool,gx::vector3> boxBox(const BoundingBox * a,const  BoundingBox * b);
std::pair<bool,gx::vector3> boxRay(const BoundingBox * a,const  Ray * b);
std::pair<bool,gx::vector3> sphereSphere(const BoundingSphere * a,const  BoundingSphere * b);
std::pair<bool,gx::vector3> sphereRay(const BoundingSphere * a,const  Ray* b);
void boxTest();
void sphereTest();
