#pragma once
#include "graphics/vector4.h"
#include "graphics/vector3.h"
#include "Rectangle.h"

class Quadtree;

class BoundingObj{
private:
  Rectangle* rect;
  Quadtree* qtree;

public:
  BoundingObj():rect(nullptr)/*,qtree(nullptr)*/{
    rect = new Rectangle();
  }
  Quadtree* getQtree()const{return qtree;}
  void setQuadtree(Quadtree* q){qtree=q;}
  Rectangle* getRect()const{return rect;}
  void setRect(Rectangle* r){rect = r;}
  
  bool operator==(const BoundingObj &other){
    return other.rect == rect;
  }
  bool operator!=(const BoundingObj &other){
    return other.rect != rect;
  }
};