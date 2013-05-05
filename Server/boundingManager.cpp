#include "boundingManager.h"

//////////////////////helper functions/////////////////
bool sortEntity(std::pair<Entity*,gx::vector3> a, std::pair<Entity*,gx::vector3> b){
  return a.second.magnitudesq() < b.second.magnitudesq();
}

int sign( float number)
{
  if(number > 0)
    return 1;
  if(number < 0)
    return -1;
  return 0;
}

//returns true if separated by given axis (axis),
//if false, also gives the vector to separate a from b
//t is vector between centers
std::pair<bool,gx::vector3> notSeparatedByAxis(const BoundingBox* a, const gx::vector3 t,
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

  bool retBool = !(centerDist > sumDist);
  gx::vector3 ret = axis;
  if(retBool){
    //calculate the return vec
    ret.normalize();
    ret.scale( sumDist - centerDist );   
    if( sign( t.dot(axis) ) < 0 ){
      ret.negate();
    }
  }

  return std::pair<bool,gx::vector3>(retBool, ret);
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
std::pair<bool,gx::vector3> boxBox(const BoundingBox* a,const  BoundingBox * b){
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

  gx::vector3 axes[15];
  axes[0] = ax; axes[1] = ay; axes[2] = az;
  axes[3] = bx; axes[4] = by; axes[5] = bz;
  axes[6] = axbx; axes[7] = axby; axes[8] = axbz;
  axes[9] = aybx; axes[10] = ayby; axes[11] = aybz;
  axes[12] = azbx; axes[13] = azby; axes[14] = azbz;

  std::pair<bool, gx::vector3> min;
  std::pair<bool, gx::vector3> ret;
  bool firstTime = true;

  for( int i = 0; i < 15; i++){
    //dont try for axis 0,0,0
    if( axes[i].x == 0 && axes[i].y == 0 && axes[i].z == 0)
      continue;

    //init min
    if( firstTime ){
      min = notSeparatedByAxis(a,t,axes[i],b);
      firstTime = false;
      continue;
    }
    else{
      ret = notSeparatedByAxis(a,t,axes[i],b);
    }

    //determin min
    if( !ret.first )
      return std::pair<bool,gx::vector3>(false,gx::vector3());
    else{
      if(ret.second.magnitudesq() < min.second.magnitudesq())
        min.second = ret.second;
    }
  }
  return ret;
}

std::pair<bool,gx::vector3> boxRay(const BoundingBox* b,const Ray* r){
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
    centerBox.dot(ax) - hw, centerBox.dot(ax) + hw, tfirst, tlast)) 
    return std::pair<bool,gx::vector3>(false,gx::vector3());
  if (!raySlab(b,origin.dot(ay), r->getDirection().dot(ay), 
    centerBox.dot(ay) - hh, centerBox.dot(ay) + hh, tfirst, tlast))
    return std::pair<bool,gx::vector3>(false,gx::vector3());
  if (!raySlab(b,origin.dot(az), r->getDirection().dot(az), 
    centerBox.dot(az) - hd, centerBox.dot(az) + hd, tfirst, tlast))
    return std::pair<bool,gx::vector3>(false,gx::vector3());

  //the time it hit
  t = tfirst;
  gx::vector3 ret = r->getDirection();
  ret.scale(1.0-tfirst);
  //ret.scale(1.0-tfirst);
  //ret.negate();

  return std::pair<bool,gx::vector3>(true,ret);
}

std::pair<bool,gx::vector3> sphereSphere(const BoundingSphere* o, const BoundingSphere* t){
  gx::vector3 v3 = t->getCenter() - o->getCenter();
  
  BoundingObj::unit_t mag = v3.magnitude();
  BoundingObj::unit_t radSum = o->getRadius() + t->getRadius();
  v3.normalize();
  v3.scale( radSum - mag );

  return std::pair<bool, gx::vector3>( mag < radSum, v3);
}

std::pair<bool,gx::vector3> sphereRay(const BoundingSphere* s, const Ray* r){
  //project the center onto the line of the ray
  float ab2 = r->getDirection().dot(r->getDirection());
  gx::vector3 ap = s->getCenter() - r->getOrigin();
  float ap_dot_ab = ap.dot(r->getDirection());
  float t = ap_dot_ab / ab2;

  if( t < 0.0f ) t = 0.0f;
  else if( t > 1.0f ) t = 1.0f;

  //q is closest point
  gx::vector4 q = r->getOrigin() + r->getDirection() * t;
  
  //check if that point is in the circle
  gx::vector3 pq = q - s->getCenter();
  float pq2 = pq.dot(pq);
  float r2 = s->getRadius() * s->getRadius();

  //calculate return vec
  gx::vector3 ret = r->getDirection();
  ret.scale(1.0-t);
  //ret.scale(1.0-t);
  //ret.negate();
  return std::pair<bool, gx::vector3>(pq2<=r2, ret);
}

std::pair<bool,gx::vector3> collide(const BoundingObj * a,const  BoundingObj * b){
  if(a->isBox() && b->isBox())
    return boxBox((const BoundingBox*)a,(const BoundingBox*)b);
  else if(a->isBox() && b->isRay())
    return boxRay((const BoundingBox*)a,(const Ray*)b);
  else if(b->isBox() && a->isRay()){
    std::pair<bool,gx::vector3> r = boxRay((const BoundingBox*)b,(const Ray*)a);
    r.second.negate();
    return r;
  }
  
  else if(a->isSphere() && b->isSphere())
    return sphereSphere((const BoundingSphere*)a,(const BoundingSphere*)b);
  else if(a->isSphere() && b->isRay())
    return sphereRay((const BoundingSphere*)a,(const Ray*)b);
  else if(b->isSphere() && a->isRay()){
    std::pair<bool,gx::vector3> r = sphereRay((const BoundingSphere*)b,(const Ray*)a);
    r.second.negate();
    return r;
  }

  return std::pair<bool,gx::vector3>(false,gx::vector3());
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
  std::cout << "1==" << collide(&b1,&b2).first << collide(&b2,&b1).first << std::endl;
    std::cout << "1==" << collide(&b1,&b2).second << collide(&b2,&b1).second << std::endl;
  std::cout << "1==" << collide(&b3,&b4).first << collide(&b4,&b3).first << std::endl;
    std::cout << "1==" << collide(&b3,&b4).second << collide(&b4,&b3).second << std::endl;
  std::cout << "0==" << collide(&b3,&b5).first << collide(&b5,&b3).first << std::endl;
    std::cout << "0==" << collide(&b3,&b5).second << collide(&b5,&b3).second << std::endl;

  Ray r1(gx::vector4(-50,0,0), gx::vector3(100,0,0));
  Ray r2(gx::vector4(0,0,0), gx::vector3(1000,1000,0));
  std::cout << "1==" << collide(&b1,&r1).first << collide(&r1,&b1).first << std::endl;
    std::cout << "1==" << collide(&b1,&r1).second << collide(&r1,&b1).second << std::endl;
  std::cout << "1==" << collide(&b3,&r1).first << collide(&r1,&b3).first << std::endl;
    std::cout << "1==" << collide(&b3,&r1).second << collide(&r1,&b3).second << std::endl;
  std::cout << "0==" << collide(&b5,&r2).first << collide(&b5,&r2).first << std::endl;
    std::cout << "0==" << collide(&b5,&r2).second << collide(&b5,&r2).second << std::endl;
}

void sphereTest(){
  BoundingSphere s1(gx::vector4(0,0,0),100);
  BoundingSphere s2(gx::vector4(109,0,0),10);
  BoundingSphere s3(gx::vector4(0,10,0),10);
  std::cout << "true 1==" << collide(&s1, &s2).first << collide(&s2, &s1).first << std::endl;
    std::cout << "true 1==" << collide(&s1, &s2).second << collide(&s2, &s1).second << std::endl;
  std::cout << "true 1==" << collide(&s1, &s3).first << collide(&s3, &s1).first << std::endl;
    std::cout << "true 1==" << collide(&s1, &s3).second << collide(&s3, &s1).second << std::endl;
  std::cout << "false 0==" << collide(&s3, &s2).first << collide(&s2, &s3).first << std::endl;
    std::cout << "false 0==" << collide(&s3, &s2).second << collide(&s2, &s3).second << std::endl;

  Ray r1(gx::vector4(109,0,0), gx::vector3(-10,0,0));
  Ray r2(gx::vector4(0,40,0), gx::vector3(0,-1000,0));
  Ray r3(gx::vector4(0,40,0), gx::vector3(0,1000,0));
  std::cout << "true 1==" << collide(&s1, &r1).first << collide(&r1, &s1).first << std::endl;
    std::cout << "true 1==" << collide(&s1, &r1).second << collide(&r1, &s1).second << std::endl;
  std::cout << "true 1==" << collide(&s3, &r2).first << collide(&r2, &s3).first << std::endl;
     std::cout << "true 1==" << collide(&s3, &r2).second << collide(&r2, &s3).second << std::endl;
  std::cout << "false 0==" << collide(&s3, &r3).first << collide(&r3, &s3).first << std::endl;
    std::cout << "false 0==" << collide(&s3, &r3).second << collide(&r3, &s3).second << std::endl;
}

