#include "boundingBox.h"
#include "Quadtree.h"
#include "graphics\matrix.h"

void BoundingBox::updateRect(){
  vec4_t c[8]; //8 corners
  vec3_t up, down, left, right, in, out;
  c[1] = c[2] = c[3] = c[4] = c[5] = c[6] = c[7] = c[0] = center;
  up = az;
  up.scale(hd);
  down = up;
  down.negate();

  in = ay;
  in.scale(hh);
  out = in;
  out.negate();

  right = ax;
  right.scale(hw);
  left = right;
  right.negate();

  c[1] = c[1] + in + down + right;
  c[2] = c[2] + in + down + left;
  c[3] = c[3] + in + up + right;
  c[4] = c[4] + in + up + left;
  
  c[5] = c[5] + out + down + right;
  c[6] = c[6] + out + down + left;
  c[7] = c[7] + out + up + right;
  c[0] = c[0] + out + up + left;

  float minX, minY, /*minZ,*/ maxX, maxY/*, maxZ*/;
  maxX = minX = c[0].x;
  maxY = minY = c[0].y;
  //maxZ = minZ = c[0].z;

  for(int i = 0; i < 8; i++){
    if( c[i].x > maxX )
      maxX = c[i].x;
    if( c[i].x < minX )
      minX = c[i].x;
    if( c[i].y > maxY )
      maxY = c[i].y;
    if( c[i].y < minY )
      minY = c[i].y;
    //if( c[i].z > maxZ )
    //  maxZ = c[i].z;
    //if( c[i].z < minZ )
    //  minZ = c[i].z;
  }

  getRect()->setCenter(vec4_t((minX+maxX)/2,(minY+maxY)/2,0));
  getRect()->setHalfWidth((maxX-minX)/2);
  getRect()->setHalfHeight((maxY-minY)/2);
}

void BoundingBox::rotate(vec3_t dir, vec3_t up){
  ConfigManager::log( "before" );
  ConfigManager::log( this->toString() );
  std::stringstream ss;
  ss << "dir " << dir;
  ConfigManager::log( ss.str() );
  dir.normalize();
  if(dir.x == 0 && dir.y == 0 && dir.z == 0)
    return;
  ay = dir;
  gx::matrix rotAndTrans = gx::translation(center.x,center.y,center.z) * toRightHandBasisFromYandUp(dir,up);
  ax = gx::multiply(rotAndTrans, ax);
  az = gx::multiply(rotAndTrans, az);
  ConfigManager::log( this->toString() );
}

std::string BoundingBox::toString(){
  std::stringstream ss;
  ss << BoundingObj::toString();
  ss << "axis x " << ax << " axis y " << ay << " axis z " << az << std::endl;
  ss << "hw " << hw << " hh " << hh << " hd " << hd << std::endl;
  return ss.str();
}

BoundingObj::vec3_t BoundingBox::getMaxRadius( vec3_t axis ){
  axis.normalize();

  vec3_t radius = ax;
  radius.scale(hw);
  unit_t maxRad = std::fabs(radius.dot(axis));

  radius = ay;
  radius.scale(hh);
  unit_t rad = std::fabs(radius.dot(axis));
  if( rad > maxRad ){
    maxRad = rad;
  }

  radius = az;
  radius.scale(hd);
  rad = std::fabs(radius.dot(axis));
  if( rad > maxRad ){
    maxRad = rad;
  }

  axis.scale(maxRad);
  return axis;
}