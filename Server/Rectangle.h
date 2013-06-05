#pragma once
#include "graphics/vector4.h"

class Rectangle{
private:
  gx::vector4f center;
  float hw;
  float hh;

public:
  Rectangle(gx::vector4f cen, float halfWidth, float halfHeight):
  center(cen), hw(halfWidth), hh(halfHeight){}
  Rectangle():center(0,0,0), hw(0), hh(0){}
  float getHalfWidth() const{ return hw; }
  float getHalfHeight() const{ return hh; }
  gx::vector4f getCenter() const{return center;}
  void setCenter(gx::vector4f cen){ center = cen; }
  void setHalfWidth(float halfWidth){ hw = halfWidth; }
  void setHalfHeight(float halfHeight){ hh = halfHeight; }
  std::string toString(){
    std::stringstream ss;
    ss << "rectangle " << std::endl
    << "center " << center << " w " << hw << " h " << 
    hh << std::endl 
    << " object " << std::endl;
    return ss.str();
  }
};
