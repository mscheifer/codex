#pragma once
#include "boundingObj.h"
#include "boundingBox.h"

class Ray: public BoundingObj{
private:
  //gx::vector4 origin;// Center == origin
  BoundingObj::vec3_t direction;

public:
  Ray(BoundingObj::vec4_t o, vec3_t d) : BoundingObj(o), direction(d){
    //origin = center;
    updateRect();
  }
  void updateRect();
  bool isRay() const{return true;}
  std::string toString();

  vec3_t getDirection() const {return direction;}
  void setDirection(vec3_t v){direction = v;}
  vec4_t getOrigin() const {return getCenter();}
  void setOrigin(vec4_t o) {center = o;}
};