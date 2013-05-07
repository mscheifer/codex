#pragma once
#include "graphics/vector4.h"
#include "graphics/vector3.h"
#include "Rectangle.h"

class Quadtree;
class Entity;
class BoundingSphere;
class BoundingBox;
class Ray;

class BoundingObj{
private:
  Quadtree* qtree;
  Entity* entity;
  
protected:
  Rectangle* rect;
  gx::vector4 center;

public:
  typedef float unit_t;
  //BoundingObj():rect(nullptr),qtree(nullptr){
  //  rect = new Rectangle();
  //}
  BoundingObj(gx::vector4 cen):center(cen),qtree(nullptr){
    rect = new Rectangle();
  }
  ~BoundingObj();

  //functions for qtree
  virtual void updateRect(){}
  void updateOnTree();
  virtual void move(const gx::vector3 & v){
    center = center+v;
    updateOnTree();
  }
  //virtual void rotate(){} TODO

  virtual bool isRay() const{return false;}
  virtual bool isSphere() const{return false;}
  virtual bool isBox() const{return false;}
  
  //compare the rect pointers
  bool operator==(const BoundingObj &other){
    return other.rect == rect;
  }
  bool operator!=(const BoundingObj &other){
    return other.rect != rect;
  }

  //setters and getters
  Entity* getEntity() const{return entity;}
  Quadtree* getQtree()const{return qtree;}
  void setQuadtree(Quadtree* q){qtree=q;}
  Rectangle* getRect()const{return rect;}
  //void setRect(Rectangle* r){rect = r;}
  gx::vector4 getCenter() const{return center;}
  void setCenter(gx::vector4 cen){  
    center = cen;
    updateOnTree();
  }
};