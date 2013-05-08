#include "boundingBox.h"
#include "Quadtree.h"

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

  getRect()->setCenter(BoundingObj::vec4_t((minX+maxX)/2,(minY+maxY)/2,0));
  getRect()->setHalfWidth((maxX-minX)/2);
  getRect()->setHalfHeight((maxY-minY)/2);
}
