#include "Ray.h"
#include "Quadtree.h"

//TODO is this right? can only move in 1 direction
//void Ray::move(gx::vector3 v){
//  setOrigorigin + v;
//  getQtree()->remove(this);
//  updateRect();
//  getQtree()->insert(this);
//}

void Ray::updateRect(){
  vec4_t endpt = getOrigin() + direction;
  float minX, minY, maxX, maxY;
  minX = maxX = getOrigin().x;
  minY = maxY = getOrigin().y;
  if( endpt.x > maxX )
    maxX = endpt.x;
  if( endpt.x < minX )
    minX = endpt.x;
  if( endpt.y > maxY )
    maxY = endpt.y;
  if( endpt.y < minY )
    minY = endpt.y;

  getRect()->setCenter(vec4_t((minX+maxX)/2,(minY+maxY)/2,0));
  getRect()->setHalfWidth((maxX-minX)/2);
  getRect()->setHalfHeight((maxY-minY)/2);
}

std::string Ray::toString(){
  std::stringstream ss;
  ss << "RAY" << std::endl;
  ss << BoundingObj::toString();
  ss << getDirection() << std::endl;
  return ss.str();
}