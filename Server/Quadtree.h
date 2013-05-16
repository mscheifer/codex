#pragma once
#include <list>
#include <vector>
#include <array>
#include <iostream>
#include "boundingObj.h"

#include "boundingSphere.h" //TODO remove for testing
#include "boundingBox.h"
#include "Ray.h"

class Quadtree{
private:
  static const int maxObjects = 10; //TODO adjust these values
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
  int size(); //TODO nothing should call this this is for testing only

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
  
  /*
  static void test(){
    Rectangle worldBounds(BoundingObj::vec4_t(0,0,0),1000,1000);
    Quadtree q(0, worldBounds);
    BoundingSphere r1(BoundingObj::vec4_t(53,101,0),50);
    BoundingSphere r1_2(BoundingObj::vec4_t(53,101,0),50);
    BoundingSphere r2(BoundingObj::vec4_t(53,150,0),50);
    BoundingSphere r3(BoundingObj::vec4_t(-400,-400,0),1);
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
    r1.move(BoundingObj::vec3_t(-104,0,0));
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
    BoundingBox b6(BoundingObj::vec4_t(0,0,-1000), 
      BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
      1000,1000,1000);
    BoundingBox b7(BoundingObj::vec4_t(7,7,-20), 
      BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
      5,5,5);
    q.insert(&b6);
    q.insert(&b7);
    q.retrieve(l,&b6);
    std::cout << l.size() << "==1 b6 cw: b7. THIS" << std::endl;
    
    q.clear();
    l.clear();

    BoundingBox b3(BoundingObj::vec4_t(20,20,0), 
      BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
      5,5,5);
    BoundingBox b4(BoundingObj::vec4_t(27,20,0), 
      BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
      5,5,5);
    BoundingBox b5(BoundingObj::vec4_t(34.3f,20,0), 
      BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
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
    b5.move(BoundingObj::vec3_t(-100,-100,1));

    l.clear();
    q.retrieve(l,&b3);
    std::cout << l.size() << "==0 b3 cw:. b5 moved" << std::endl;

    //all test
    Ray ray1(BoundingObj::vec4_t(20,20,0),BoundingObj::vec3_t(1,1,1));
    BoundingSphere s1(BoundingObj::vec4_t(20,20,0),1);
    q.insert(&ray1);
    q.insert(&s1);

    l.clear();
    q.retrieve(l,&b3);
    std::cout << l.size() << "==2 b3 cw: ray1, s1." << std::endl;
    ray1.move(BoundingObj::vec3_t(-100,-100,1));

    l.clear();
    q.retrieve(l,&b3);
    std::cout << l.size() << "==1 b3 cw: s1." << std::endl;
    
  }
  */
  
};
