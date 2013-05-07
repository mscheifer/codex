#pragma once
#include <list>
#include <vector>
#include <array>
#include <iostream>
#include "boundingObj.h"

#include "boundingSphere.h" //TODO remove for testing
#include "boundingBox.h" //TODO remove for testing
#include "Ray.h" //TODO remove for testing

class Quadtree{
private:
  static const int maxObjects = 10; //TODO this needs to not be 1
  static const int maxLevels = 5;
  int level;
  std::list<BoundingObj*> objects;
  Rectangle bounds;
  std::array<Quadtree*, 4> nodes;

  //split into 4 nodes
  void split();
  /*
   * Determine which node the object belongs to. -1 means
   * object cannot completely fit within a child node and is part
   * of the parent node
   */
  int getIndex(BoundingObj* o);

public:
  Quadtree(int pLevel, Rectangle pBounds);
  ~Quadtree();

  //clear the quadtree
  void clear();

  /*
   * Insert the object into the quadtree. If the node
   * exceeds the capacity, it will split and add all
   * objects to their corresponding nodes.
   */
  void insert(BoundingObj* o);

  //remove object from the tree
  void remove(BoundingObj* o);

  //Return all objects that could collide with the given object
  std::vector<BoundingObj*>& retrieve(std::vector<BoundingObj*> & returnObjects, BoundingObj* pRect);
  
  
  static void test(){
    Rectangle worldBounds(gx::vector4(0,0,0),1000,1000);
    Quadtree q(0, worldBounds);
    BoundingSphere r1(gx::vector4(53,101,0),50);
    BoundingSphere r1_2(gx::vector4(53,101,0),50);
    BoundingSphere r2(gx::vector4(53,150,0),50);
    BoundingSphere r3(gx::vector4(-400,-400,0),1);
    std::vector<BoundingObj*> l;
    
    //THESE TESTS only work if maxobjects == 1
    q.insert(&r1);
    q.insert(&r1_2);
    q.retrieve(l,&r1);
    std::cout << "quadtree test:" << std::endl;
    std::cout << l.size() << "==1 r1 collide with r1_2." << std::endl;
    q.insert(&r2);
    q.insert(&r3);
    l.clear();
    q.retrieve(l,&r1);
    std::cout << l.size() << "==2 r2 cw: r1, r1_2." << std::endl;
    l.clear();
    q.retrieve(l,&r3);
    std::cout << l.size() << "==0 r3 cw:." << std::endl;
    l.clear();
    q.remove(&r2);
    q.retrieve(l,&r1);
    std::cout << l.size() << "==1 r1 cw: r1_2. r2 removed" << std::endl;
    //q.clear();
    r1.move(gx::vector3(-104,0,0));
    l.clear();
    q.retrieve(l,&r1);
    std::cout << l.size() << "==0 r1 cw:. r1 moved" << std::endl;

    l.clear();
    q.clear();
    q.retrieve(l,&r1);
    std::cout << l.size() << "==0 clear" << std::endl;

    //bounding box test
    q.clear();
    l.clear();
    BoundingBox b3(gx::vector4(20,20,0), 
      gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
      5,5,5);
    BoundingBox b4(gx::vector4(27,20,0), 
      gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
      5,5,5);
    BoundingBox b5(gx::vector4(34.3f,20,0), 
      gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
      5,5,5);
    q.insert(&b3);
    q.insert(&b4);
    q.retrieve(l,&b3);
    std::cout << l.size() << "==1 b3 cw: b4." << std::endl;

    q.remove(&b4);
    q.insert(&b5);
    l.clear();
    q.retrieve(l,&b3);
    std::cout << l.size() << "==1 b3 cw: b5." << std::endl;
    b5.move(gx::vector3(-100,-100,1));

    l.clear();
    q.retrieve(l,&b3);
    std::cout << l.size() << "==0 b3 cw:. b5 moved" << std::endl;

    //all test
    Ray ray1(gx::vector4(20,20,0),gx::vector3(1,1,1));
    BoundingSphere s1(gx::vector4(20,20,0),1);
    q.insert(&ray1);
    q.insert(&s1);

    l.clear();
    q.retrieve(l,&b3);
    std::cout << l.size() << "==2 b3 cw: ray1, s1." << std::endl;
    ray1.move(gx::vector3(-100,-100,1));

    l.clear();
    q.retrieve(l,&b3);
    std::cout << l.size() << "==1 b3 cw: s1." << std::endl;
    
  }
  
  
};
