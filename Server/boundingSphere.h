#pragma once
#include "vector4.h"
#include "vector3.h"
#include "Ray.h"
#include <math.h>
#include <iostream>

class BoundingSphere
{
private:
  gx::vector4 center;
  float radius; //TODO: change to glfloat

public:
  BoundingSphere();
  BoundingSphere(float x, float y, float z, float radius);
  void move(const gx::vector3 & v);
  bool collideWith(const BoundingSphere & o);
  bool collideWith(const Ray & r);
  ~BoundingSphere(void);
  
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
};

  /*vars for client.h
  //this is for collision detection test
  sf::CircleShape c1;
  BoundingSphere s1;
  sf::CircleShape c2;
  BoundingSphere s2;
  //end collision

  //key released cases for client.cpp
          case sf::Keyboard::Right:
          c1.setPosition(c1.getPosition().x+5 , c1.getPosition().y);
          s1.move(gx::vector3(5,0,0));
          break;
        case sf::Keyboard::Left:
          c1.setPosition(c1.getPosition().x-5 , c1.getPosition().y);
          s1.move(gx::vector3(-5,0,0));
          break;
        case sf::Keyboard::Up:
          c1.setPosition(c1.getPosition().x , c1.getPosition().y-5);
          s1.move(gx::vector3(0,0,-5));
          break;
        case sf::Keyboard::Down:
          c1.setPosition(c1.getPosition().x , c1.getPosition().y+5);
          s1.move(gx::vector3(0,0,5));
          break;
  */

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