#pragma once
#include "boundingObj.h"
#include "boundingBox.h"
#include "boundingSphere.h"
#include "Ray.h"

inline bool collide(const BoundingObj * a,const  BoundingObj * b);
inline bool boxBox(const BoundingBox * a,const  BoundingBox * b);
inline bool boxRay(const BoundingBox * a,const  Ray * b);
inline bool sphereSphere(const BoundingSphere * a,const  BoundingSphere * b);
inline bool sphereRay(const BoundingSphere * a,const  Ray* b);
inline void boxTest();
inline void sphereTest();