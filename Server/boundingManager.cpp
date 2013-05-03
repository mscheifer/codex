//#include "boundingManager.h" //TODO fix thsi wtf
#include "boundingObj.h"
#include "boundingBox.h"
#include "boundingSphere.h"
#include "Ray.h"

//////////////////////helper functions/////////////////

//returns true if separated by given axis (axis)
//t is vector between centers
bool separatedByAxis(const BoundingBox* a, const gx::vector3 t,
  const gx::vector3 axis, const BoundingBox* b){

  float centerDist = abs(t.dot(axis));
  float sumDist = 0;

  //Ax*Aw dot L
  gx::vector3 axis2 = a->getAx();
  axis2.scale(a->getHw());
  sumDist += abs(axis2.dot(axis));

  //Ay*Ah dot L
  axis2 = a->getAy();
  axis2.scale(a->getHh());
  sumDist += abs(axis2.dot(axis));

  //Az*Ad dot L
  axis2 = a->getAz();
  axis2.scale(a->getHd());
  sumDist += abs(axis2.dot(axis));

  //Bx*Bw dot L
  axis2 = b->getAx();
  axis2.scale(b->getHw());
  sumDist += abs(axis2.dot(axis));

  //By*Bh dot L
  axis2 = b->getAy();
  axis2.scale(b->getHh());
  sumDist += abs(axis2.dot(axis));

  //Bz*Bd dot L
  axis2 = b->getAz();
  axis2.scale(b->getHd());
  sumDist += abs(axis2.dot(axis));

  return centerDist > sumDist;
}

bool raySlab(const BoundingBox* a, float start, float dir,
  float min, float max, float& tfirst, float& tlast){

  if (fabs(dir) < 1.0E-8)
  {
    return (start < max && start > min);
  }

  float tmin = (min - start) / dir;
  float tmax = (max - start) / dir;
  //swap
  if (tmin > tmax){
    float temp = tmin;
    tmin = tmax;
    tmax = temp;
  }
   
  if (tmax < tfirst || tmin > tlast)
      return false;

  if (tmin > tfirst) tfirst = tmin;
  if (tmax < tlast)  tlast  = tmax;
  return true;
}

/////////////////////end helpers///////////////////////
bool boxBox(const BoundingBox* a,const  BoundingBox * b){
  gx::vector3 axbx, axby, axbz, aybx, ayby, aybz, azbx, azby, azbz;
  gx::vector3 ax = a->getAx(), 
    bx = b->getAx(),
    ay = a->getAy(),
    by = b->getAy(),
    az = a->getAz(),
    bz = b->getAz();

  //the vector between the centers
  gx::vector3 t = b->getCenter() - a->getCenter();

  axbx.cross(ax,bx);
  axby.cross(ax,by);
  axbz.cross(ax,bz);
  
  aybx.cross(ay,bx);
  ayby.cross(ay,by);
  aybz.cross(ay,bz);
  
  azbx.cross(az,bx);
  azby.cross(az,by);
  azbz.cross(az,bz);

  return !separatedByAxis(a,t,ax,b) && !separatedByAxis(a,t,ay,b) && !separatedByAxis(a,t,az,b) &&
    !separatedByAxis(a,t,bx,b) && !separatedByAxis(a,t,by,b) && !separatedByAxis(a,t,bz,b) &&
    !separatedByAxis(a,t,axbx,b) && !separatedByAxis(a,t,axby,b) && !separatedByAxis(a,t,axbz,b) &&
    !separatedByAxis(a,t,aybx,b) && !separatedByAxis(a,t,ayby,b) && !separatedByAxis(a,t,aybz,b) &&
    !separatedByAxis(a,t,azbx,b) && !separatedByAxis(a,t,azby,b) && !separatedByAxis(a,t,azbz,b);
}

bool boxRay(const BoundingBox* b,const Ray* r){
  gx::vector3 ax = b->getAx(), 
  ay = b->getAy(),
  az = b->getAz();
  BoundingObj::unit_t hw = b->getHw(),
    hh = b->getHh(),
    hd = b->getHd();

  float t = 0;
  float tfirst = 0.0f, tlast = 1.0f;

  //convert origin to vector3
  gx::vector3 origin(r->getOrigin().x,r->getOrigin().y,r->getOrigin().z);
  gx::vector3 centerBox(b->getCenter().x, b->getCenter().y, b->getCenter().z);

  if (!raySlab(b,origin.dot(ax), r->getDirection().dot(ax), 
    centerBox.dot(ax) - hw, centerBox.dot(ax) + hw, tfirst, tlast)) return false;
  if (!raySlab(b,origin.dot(ay), r->getDirection().dot(ay), 
    centerBox.dot(ay) - hh, centerBox.dot(ay) + hh, tfirst, tlast)) return false;
  if (!raySlab(b,origin.dot(az), r->getDirection().dot(az), 
    centerBox.dot(az) - hd, centerBox.dot(az) + hd, tfirst, tlast)) return false;

  //the time it hit
  t = tfirst;
  return true;
}

bool sphereSphere(const BoundingSphere* t, const BoundingSphere* o){
  gx::vector3 v3 = gx::vector3(o->getCenter().x - t->getCenter().x,
    o->getCenter().y - t->getCenter().y,
    o->getCenter().z - t->getCenter().z);
  
  //v3 is direction vector and distance to separate is r1 + r2 - v3.mag
  if(v3.magnitude() < o->getRadius() + t->getRadius()){
    //std::cout << "direction: x "<< v3.x << " y "<< v3.y << " z "<< v3.z << " \n dist " << (o->radius+radius)-v3.magnitude() << std::endl;
  }
  return v3.magnitude() < o->getRadius() + t->getRadius();
}

bool sphereRay(const BoundingSphere* s, const Ray* r){
  //project the center onto the line of the ray
  float ab2 = r->getDirection().dot(r->getDirection());
  gx::vector3 ap = s->getCenter() - r->getOrigin();
  float ap_dot_ab = ap.dot(r->getDirection());
  float t = ap_dot_ab / ab2;

  if( t < 0.0f ) t = 0.0f;
  else if( t > 1.0f ) t = 1.0f;

  //q is closest point
  gx::vector4 q = r->getOrigin() + r->getDirection() * t;
  std::cout << "collide at point " << q.x << "," << q.y << "," << q.z << std::endl;
  
  //check if that point is in the circle
  gx::vector3 pq = q - s->getCenter();
  float pq2 = pq.dot(pq);
  float r2 = s->getRadius() * s->getRadius();
  return pq2 <= r2;
}

bool collide(const BoundingObj * a,const  BoundingObj * b){
  if(a->isBox() && b->isBox())
    return boxBox((const BoundingBox*)a,(const BoundingBox*)b);
  else if(a->isBox() && b->isRay())
    return boxRay((const BoundingBox*)a,(const Ray*)b);
  else if(b->isBox() && a->isRay())
    return boxRay((const BoundingBox*)b,(const Ray*)a);
  
  else if(a->isSphere() && b->isSphere())
    return sphereSphere((const BoundingSphere*)a,(const BoundingSphere*)b);
  else if(a->isSphere() && b->isRay())
    return sphereRay((const BoundingSphere*)a,(const Ray*)b);
  else if(b->isSphere() && a->isRay())
    return sphereRay((const BoundingSphere*)b,(const Ray*)a);

  return false;
}

void boxTest(){
  BoundingBox b1(gx::vector4(0,0,0), 
    gx::vector3(1,0,0), gx::vector3(0,1,0), gx::vector3(0,0,1),
    5,5,5);
  BoundingBox b2(gx::vector4(1,0,0), 
    gx::vector3(1,0,0), gx::vector3(0,1,0), gx::vector3(0,0,1),
    5,5,5);
    
  BoundingBox b3(gx::vector4(0,0,0), 
    gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
    5,5,5);
  BoundingBox b4(gx::vector4(7,0,0), 
    gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
    5,5,5);
  BoundingBox b5(gx::vector4(14.3,0,0), 
    gx::vector3(1,1,0), gx::vector3(1,-1,0), gx::vector3(0,0,1),
    5,5,5);
  std::cout << "1==" << collide(&b1,&b2) << collide(&b2,&b1) << std::endl;
  std::cout << "1==" << collide(&b3,&b4) << collide(&b4,&b3) << std::endl;
  std::cout << "0==" << collide(&b3,&b5) << collide(&b5,&b3) << std::endl;

  Ray r1(gx::vector4(-50,0,0), gx::vector3(100,0,0));
  Ray r2(gx::vector4(0,0,0), gx::vector3(1000,1000,0));
  std::cout << "1==" << collide(&b1,&r1) << collide(&r1,&b1) << std::endl;
  std::cout << "1==" << collide(&b3,&r1) << collide(&r1,&b3) << std::endl;
  std::cout << "0==" << collide(&b5,&r2) << collide(&b5,&r2) << std::endl;
}

void sphereTest(){
  BoundingSphere s1(gx::vector4(0,0,0),100);
  BoundingSphere s2(gx::vector4(109,0,0),10);
  BoundingSphere s3(gx::vector4(0,10,0),10);
  std::cout << "true 1==" << collide(&s1, &s2) << collide(&s2, &s1) << std::endl;
  std::cout << "true 1==" << collide(&s1, &s3) << collide(&s3, &s1) << std::endl;
  std::cout << "false 0==" << collide(&s3, &s2) << collide(&s2, &s3) << std::endl;

  Ray r1(gx::vector4(109,0,0), gx::vector3(-10,0,0));
  Ray r2(gx::vector4(0,40,0), gx::vector3(0,-1000,0));
  Ray r3(gx::vector4(0,40,0), gx::vector3(0,1000,0));
  std::cout << "true 1==" << collide(&s1, &r1) << collide(&r1, &s1) << std::endl;
  std::cout << "true 1==" << collide(&s3, &r2) << collide(&r2, &s3) << std::endl;
  std::cout << "false 0==" << collide(&s3, &r3) << collide(&r3, &s3) << std::endl;
}