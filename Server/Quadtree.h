#pragma once
#include <list>
#include <array>
#include <iostream>
#include "boundingObj.h"

#include "boundingSphere.h" //TODO remove this is for testing

class Quadtree{
private:
  static const int maxObjects = 1; //TODO this needs to not be 1
  static const int maxLevels = 5;
  int level;
  std::list<BoundingObj> objects;
  Rectangle bounds;
  std::array<Quadtree*, 4> nodes;
  void split();
  int getIndex(BoundingObj o);

public:
  Quadtree(int pLevel, Rectangle pBounds);
  ~Quadtree();
  void clear();
  void insert(BoundingObj o);
  void remove(BoundingObj o);
  std::list<BoundingObj>& retrieve(std::list<BoundingObj> & returnObjects, BoundingObj pRect);
  
  
  static void test(){
    Rectangle worldBounds(gx::vector4(0,0,0),1000,1000);
    Quadtree q(0, worldBounds);
    BoundingSphere r1(gx::vector4(53,101,0),50);
    BoundingSphere r2(gx::vector4(53,150,0),50);
    BoundingSphere r3(gx::vector4(-400,-400,0),1);

    q.insert(r1);
    std::list<BoundingObj> l;
    q.retrieve(l,r1);
    std::cout << "quadtree test:" << std::endl;
    std::cout << "should be 1 " << l.size() << std::endl;
    q.insert(r2);
    q.insert(r3);
    l.clear();
    q.retrieve(l,r1);
    std::cout << "should be 2 " << l.size() << std::endl;
    l.clear();
    q.retrieve(l,r3);
    std::cout << "should be 1 " << l.size() << std::endl;
    l.clear();
    q.remove(r2);
    q.retrieve(l,r1);
    std::cout << "should be 1 " << l.size() << std::endl;
    q.clear();
    l.clear();
    q.retrieve(l,r1);
    std::cout << "should be 0 " << l.size() << std::endl;
  }
  
};