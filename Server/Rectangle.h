#pragma once
#include "graphics/vector4.h"

class Rectangle{
private:
  gx::vector4 center;
  float hw;
  float hh;

public:
  Rectangle(gx::vector4 cen, float halfWidth, float halfHeight):
  center(cen), hw(halfWidth), hh(halfHeight){}
  Rectangle():center(0,0,0), hw(0), hh(0){}
  float getHalfWidth() const{ return hw; }
  float getHalfHeight() const{ return hw; }
  gx::vector4 getCenter() const{return center;}
  void setCenter(gx::vector4 cen){ center = cen; }
  void setHalfWidth(float halfWidth){ hw = halfWidth; }
  void setHalfHeight(float halfHeight){ hh = halfHeight; }
};
