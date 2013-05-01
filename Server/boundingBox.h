#pragma once
#include "vector4.h"
#include "vector3.h"
#include "Ray.h"
#include <math.h>
#include <iostream>

class BoundingBox{
private:
  gx::vector4 center;
  //these vectors should be normalized
  gx::vector3 ax;
  gx::vector3 ay;
  gx::vector3 az;
  float hw;  //half width ax
  float hh;  //half height ay
  float hd;  //half depth az

  //true if they are separated by the axis axis
  bool separatedByAxis(const gx::vector3 t, const gx::vector3 axis, const BoundingBox & b);
  bool raySlab(float start, float dir, float min, float max, float& tfirst, float& tlast);

public:
  BoundingBox(gx::vector4 c, gx::vector3 x, gx::vector3 y, gx::vector3 z,
    float width, float height, float depth) : 
    center(c), ax(x), ay(y),az(z),hw(width),hh(height),hd(depth){
      ax.normalize();
      ay.normalize();
      az.normalize();
  }

  bool collideWith(const BoundingBox & b);
  bool collideWith(const Ray & r);

  static void test(){
    BoundingBox b1(gx::vector4(0,0,0), 
      gx::vector3(1,0,0), gx::vector3(0,1,0), gx::vector3(0,0,1),
      5,5,5);
    BoundingBox b2(gx::vector4(1,0,0), 
      gx::vector3(1,0,0), gx::vector3(0,1,0), gx::vector3(0,0,1),
      5,5,5);
    
    BoundingBox b3(gx::vector4(0,0,0), 
      gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
      5,5,5);
    BoundingBox b4(gx::vector4(7,0,0), 
      gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
      5,5,5);
    BoundingBox b5(gx::vector4(14.3,0,0), 
      gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
      5,5,5);
    std::cout << "true " << b1.collideWith(b2) << std::endl;
    std::cout << "true " << b3.collideWith(b4) << std::endl;
    std::cout << "false: " << b3.collideWith(b5) << std::endl;

    Ray r1(gx::vector4(-50,0,0), gx::vector3(100,0,0));
    Ray r2(gx::vector4(0,0,0), gx::vector3(1000,1000,0));
    std::cout << "true " << b1.collideWith(r1) << std::endl;
    std::cout << "true " << b3.collideWith(r1) << std::endl;
    std::cout << "false: " << b5.collideWith(r2) << std::endl;
  }
};