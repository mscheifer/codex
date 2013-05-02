#pragma once
#include "boundingObj.h"

class Ray: public BoundingObj{
private:
  gx::vector4 origin;
  gx::vector3 direction;

public:
  Ray(gx::vector4 o, gx::vector3 d) : origin(o), direction(d){
    updateRect();
  }
  gx::vector3 getDirection() const {
    return direction;
  }

  gx::vector4 getOrigin() const {
    return origin;
  }
  void move(gx::vector3 v);
  void updateRect();
};