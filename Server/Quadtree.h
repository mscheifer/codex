#pragma once
#include <list>
#include <array>
#include "Rectangle.h"
#include <iostream>
#define MAX_OBJECTS 10
#define MAX_LEVELS 5

class Quadtree{
private:
  int level;
  std::list<Rectangle> objects;
  Rectangle bounds;
  std::array<Quadtree*, 4> nodes;
  void split();
  int getIndex(Rectangle pRect);

public:
  Quadtree(int pLevel, Rectangle pBounds);
  ~Quadtree();
  void clear();
  void insert(Rectangle pRect);
  void remove(Rectangle pRect);
  std::list<Rectangle>& retrieve(std::list<Rectangle> & returnObjects, Rectangle pRect);
  static void test(){
    Rectangle worldBounds(0,0,1000,1000);
    Quadtree q(0, worldBounds);
    Rectangle r1(0,100,100,100);
    Rectangle r2(0,90,100,100);
    Rectangle r3(400,400,1,1);

    q.insert(r1);
    std::list<Rectangle> l;
    q.retrieve(l,r1);
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