#pragma once
#include "graphics/vector4.h"
#include "graphics/vector3.h"

class Ray{
private:
  gx::vector4 origin;
  gx::vector3 direction;

public:
  Ray(gx::vector4 o, gx::vector3 d) : origin(o), direction(d){}
  gx::vector3 getDirection() const {
    return direction;
  }

  gx::vector4 getOrigin() const {
    return origin;
  }
};