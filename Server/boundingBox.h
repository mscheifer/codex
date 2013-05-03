#pragma once
#include "graphics/vector4.h"
#include "graphics/vector3.h"
#include "Ray.h"
#include "boundingObj.h"
#include <math.h>
#include <iostream>

class BoundingBox : public BoundingObj{
private:
  //these vectors should be normalized
  gx::vector3 ax;
  gx::vector3 ay;
  gx::vector3 az;
  unit_t hw;  //half width ax
  unit_t hh;  //half height ay
  unit_t hd;  //half depth az

  void updateRect();

public:
  BoundingBox(gx::vector4 c, gx::vector3 x, gx::vector3 y, gx::vector3 z,
    unit_t width, unit_t height, unit_t depth) : 
    BoundingObj(c), ax(x), ay(y),az(z),hw(width),hh(height),hd(depth){
      ax.normalize();
      ay.normalize();
      az.normalize();
      updateRect();
  } 

  bool isBox() const{return true;}

  gx::vector3 getAx() const{ return ax; }
  gx::vector3 getAy() const{ return ay; }
  gx::vector3 getAz() const{ return az; }
  unit_t getHw() const{ return hw; }
  unit_t getHh() const{ return hh; }
  unit_t getHd() const{ return hd; }

  /*
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
  */
};