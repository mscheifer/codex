#pragma once
#include "Ray.h"
#include "boundingObj.h"
#include <math.h>
#include <iostream>

class BoundingSphere : public BoundingObj{
private:
  unit_t radius; //TODO: change to glfloat

public:
  BoundingSphere(vec4_t cen, float r) : BoundingObj(cen), radius(r) {
    updateRect();
  }
  ~BoundingSphere(void);
  
  void updateRect();
  bool isSphere() const{return true;}

  unit_t getRadius()const{return radius;}
  
  /*
  static void test(){
    BoundingSphere s1(0,0,0,100);
    BoundingSphere s2(109,0,0,10);
    BoundingSphere s3(0,10,0,10);
    std::cout << "true " << s1.collideWith(s2) << std::endl;
    std::cout << "true " << s1.collideWith(s3) << std::endl;
    std::cout << "false " << s3.collideWith(s2) << std::endl;

    Ray r1(gx::vector4(109,0,0), gx::vector3(-10,0,0));
    Ray r2(gx::vector4(0,40,0), gx::vector3(0,-1000,0));
    Ray r3(gx::vector4(0,40,0), gx::vector3(0,1000,0));
    std::cout << "true " << s1.collideWith(r1) << std::endl;
    std::cout << "true " << s3.collideWith(r2) << std::endl;
    std::cout << "false " << s3.collideWith(r3) << std::endl;
  }
  */
};