#pragma once
#include "graphics/vector4.h"
#include "graphics/vector3.h"
#include <math.h>

class boundingSphere
{
private:
  gx::vector4 center;
  float radius; //TODO: change to glfloat

public:
  boundingSphere();
  boundingSphere(float x, float y, float z, float radius);
  void boundingSphere::move(const gx::vector3 & v);
  bool collideWith(boundingSphere & o);
  ~boundingSphere(void);
};


  /*
  //testing collisions initialize
  c1 = sf::CircleShape(10.f);
  c1.setPosition(0,0);
  c1.setFillColor(sf::Color::Blue);
  s1 = boundingSphere(10,0,10,10);

  c2 = sf::CircleShape(10.f);
  c2.setPosition(25,25);
  c2.setFillColor(sf::Color::Blue);
  s2 = boundingSphere(35,0,35,10);

    //////comment out update window and use this instead
      window.clear();
    if(s1.collideWith(s2)){
      c1.setFillColor(sf::Color::Red);
    }
    else
      c1.setFillColor(sf::Color::Blue);
    window.draw(c1);
    window.draw(c2);
    window.display();
  // end of testing collision
  */