#include "Ray.h"
#include "Quadtree.h"

//TODO is this right? can only move in 1 direction
void Ray::move(gx::vector3 v){
  origin = origin + v;
  getQtree()->remove(*this);
  updateRect();
  getQtree()->insert(*this);
}

void Ray::updateRect(){
  gx::vector4 endpt = origin + direction;
  float minX, minY, maxX, maxY;
  minX = maxX = origin.x;
  minY = maxY = origin.y;
  if( endpt.x > maxX )
    maxX = endpt.x;
  if( endpt.x < minX )
    minX = endpt.x;
  if( endpt.y > maxY )
    maxY = endpt.y;
  if( endpt.y < minY )
    minY = endpt.y;

  getRect()->setCenter(gx::vector4((minX+maxX)/2,(minY+maxY)/2,0));
  getRect()->setHalfWidth((maxX-minX)/2);
  getRect()->setHalfHeight((maxY-minY)/2);
}