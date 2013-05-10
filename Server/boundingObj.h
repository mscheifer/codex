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
public:
  typedef float unit_t;
  typedef gx::vector3f vec3_t; //TODO use ints!!!
  typedef gx::vector4 vec4_t;

private:
  Quadtree* qtree;
  Entity* entity;
  
protected:
  Rectangle* rect;
  vec4_t center;

public:
  //BoundingObj():rect(nullptr),qtree(nullptr){
  //  rect = new Rectangle();
  //}
  BoundingObj(vec4_t cen):qtree(nullptr),center(cen){
    rect = new Rectangle();
  }
  ~BoundingObj();

  //functions for qtree
  virtual void updateRect(){}
  void updateOnTree();
  virtual void move(const vec3_t & v){
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
  void setEntity(Entity* e){entity = e;}
  Quadtree* getQtree()const{return qtree;}
  void setQuadtree(Quadtree* q){qtree=q;}
  Rectangle* getRect()const{return rect;}
  //void setRect(Rectangle* r){rect = r;}
  vec4_t getCenter() const{return center;}
  void setCenter(vec4_t cen){
    center = cen;
  }
  void setCenterOnTree(vec4_t cen){
    setCenter(cen);
    updateOnTree();
  }
};
