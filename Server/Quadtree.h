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
  void insert(BoundingObj& o);
  void remove(BoundingObj& o);
  std::list<BoundingObj>& retrieve(std::list<BoundingObj> & returnObjects, BoundingObj pRect);
  
  
  static void test(){
    Rectangle worldBounds(gx::vector4(0,0,0),1000,1000);
    Quadtree q(0, worldBounds);
    BoundingSphere r1(gx::vector4(53,101,0),50);
    BoundingSphere r1_2(gx::vector4(53,101,0),50);
    BoundingSphere r2(gx::vector4(53,150,0),50);
    BoundingSphere r3(gx::vector4(-400,-400,0),1);

    //THESE TESTS only work if maxobjects == 1
    q.insert(r1);
    q.insert(r1_2);
    std::list<BoundingObj> l;
    
    q.retrieve(l,r1);
    std::cout << "quadtree test:" << std::endl;
    std::cout << l.size() << "==1 r1 collide with r1_2." << std::endl;
    q.insert(r2);
    q.insert(r3);
    l.clear();
    q.retrieve(l,r1);
    std::cout << l.size() << "==2 r2 cw: r1, r1_2." << std::endl;
    l.clear();
    q.retrieve(l,r3);
    std::cout << l.size() << "==0 r3 cw:." << std::endl;
    l.clear();
    q.remove(r2);
    q.retrieve(l,r1);
    std::cout << l.size() << "==1 r1 cw: r1_2." << std::endl;
    q.clear();
    
    r1.move(gx::vector3(51,0,0));
    l.clear();
    q.retrieve(l,r1);
    std::cout << l.size() << "==0 r1 cw:." << std::endl;

    l.clear();
    q.retrieve(l,r1);
    std::cout << l.size() << "==0 clear" << std::endl;
  }
  
};