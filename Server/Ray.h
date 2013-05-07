#pragma once
#include "boundingObj.h"
#include "boundingBox.h"

class Ray: public BoundingObj{
private:
  //gx::vector4 origin;// Center == origin
  gx::vector3 direction;

public:
  Ray(gx::vector4 o, gx::vector3 d) : BoundingObj(o), direction(d){
    //origin = center;
    updateRect();
  }
  void updateRect();
  bool isRay() const{return true;}

  gx::vector3 getDirection() const {return direction;}
  gx::vector4 getOrigin() const {return getCenter();}
  void setOrigin(gx::vector4 o) {center = o;}
};