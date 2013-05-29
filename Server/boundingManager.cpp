#include "boundingManager.h"
#include "Entity.h"

//////////////////////helper functions/////////////////
//return true if b goes before a
bool sortEntity(std::pair<Entity*,BoundingObj::vec3_t> a, std::pair<Entity*,BoundingObj::vec3_t> b){
  if( a.first->getType() == WALL && b.first->getType() != WALL){ //set walls at the end
    return false;
  }
  if( b.first->getType() == WALL && a.first->getType() != WALL){ //set walls at the end
    return true;
  }
  return a.second.magnitudesq() > b.second.magnitudesq();
}

bool sortRayCollision(RayCollision a, RayCollision b){
  return a.tfirst < b.tfirst;
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
std::pair<bool,BoundingObj::vec3_t> notSeparatedByAxis(const BoundingBox* a, const BoundingObj::vec3_t t,
  const BoundingObj::vec3_t axis, const BoundingBox* b){

  float centerDist = fabs(t.dot(axis)); //MATT C LOOK AT THIS, IT CONVERTS TO INT AND ROUNDS THE NUMBER
  float sumDist = 0;

  //Ax*Aw dot L
  BoundingObj::vec3_t axis2 = a->getAx();
  axis2.scale(a->getHw());
  sumDist += fabs(axis2.dot(axis));

  //Ay*Ah dot L
  axis2 = a->getAy();
  axis2.scale(a->getHh());
  sumDist += fabs(axis2.dot(axis));

  //Az*Ad dot L
  axis2 = a->getAz();
  axis2.scale(a->getHd());
  sumDist += fabs(axis2.dot(axis));

  //Bx*Bw dot L
  axis2 = b->getAx();
  axis2.scale(b->getHw());
  sumDist += fabs(axis2.dot(axis));

  //By*Bh dot L
  axis2 = b->getAy();
  axis2.scale(b->getHh());
  sumDist += fabs(axis2.dot(axis));

  //Bz*Bd dot L
  axis2 = b->getAz();
  axis2.scale(b->getHd());
  sumDist += fabs(axis2.dot(axis));

  bool retBool = !(centerDist >= sumDist);
  BoundingObj::vec3_t ret = axis;
  if(retBool){
    //calculate the return vec
    ret.normalize();
    ret.scale( sumDist - centerDist );   
    if( t.dot(axis) < 0 ){
      ret.negate();
    }
  }

  return std::pair<bool,BoundingObj::vec3_t>(retBool, ret);
}

//start = start of ray
//dir   = direction of ray projected on axis
//min = start of axis
//max   = end of axis
//tfirst = time of first impact
//tlast  = time of last impact
bool raySlab(float start, float dir,
  float min, float max, float& tfirst, float& tlast) {

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
std::pair<bool,BoundingObj::vec3_t> boxBox(const BoundingBox* a,const  BoundingBox * b){
  BoundingObj::vec3_t axbx, axby, axbz, aybx, ayby, aybz, azbx, azby, azbz;
  BoundingObj::vec3_t ax = a->getAx(), 
    bx = b->getAx(),
    ay = a->getAy(),
    by = b->getAy(),
    az = a->getAz(),
    bz = b->getAz();

  //the vector between the centers
  BoundingObj::vec3_t t = a->getCenter() - b->getCenter(); //NOT SURE ON ORDER
    //TODO fix this, if the centers are the same how to displace?
  if(t.x == 0 && t.y == 0 && t.z == 0){
    t = BoundingObj::vec3_t(1,0,0);
  }

  axbx.cross(ax,bx);
  axby.cross(ax,by);
  axbz.cross(ax,bz);
  
  aybx.cross(ay,bx);
  ayby.cross(ay,by);
  aybz.cross(ay,bz);
  
  azbx.cross(az,bx);
  azby.cross(az,by);
  azbz.cross(az,bz);

  BoundingObj::vec3_t axes[15];
  axes[0] = ax; axes[1] = ay; axes[2] = az;
  axes[3] = bx; axes[4] = by; axes[5] = bz;
  axes[6] = axbx; axes[7] = axby; axes[8] = axbz;
  axes[9] = aybx; axes[10] = ayby; axes[11] = aybz;
  axes[12] = azbx; axes[13] = azby; axes[14] = azbz;

  std::pair<bool, BoundingObj::vec3_t> min = std::pair<bool,BoundingObj::vec3_t>(false,BoundingObj::vec3_t());
  std::pair<bool, BoundingObj::vec3_t> ret;
  bool firstTime = true;

  for( int i = 0; i < 15; i++){
    axes[i].normalize();
    //dont try for axis 0,0,0
    if( axes[i].x == 0 && axes[i].y == 0 && axes[i].z == 0)
      continue;

    //init min
    if( firstTime ){
      min = notSeparatedByAxis(a,t,axes[i],b); //need to make sure the bool is true
      if( min.first )
        firstTime = false;
      continue;
    }
    else{
      ret = notSeparatedByAxis(a,t,axes[i],b);
    }

    //determin min
    if( !ret.first )
      return std::pair<bool,BoundingObj::vec3_t>(false,BoundingObj::vec3_t());
    else{
      if(ret.second.magnitudesq() < min.second.magnitudesq())
        min.second = ret.second;
    }
  }
  return min;
}

std::pair<bool,BoundingObj::vec3_t> boxRay(const BoundingBox* b,const Ray* r){
  BoundingObj::vec3_t axis(0,0,0);
  BoundingObj::unit_t thit = 0;
  return boxRay(b,r,thit,axis,axis);
}

std::pair<bool,BoundingObj::vec3_t> boxRay(const BoundingBox* b,const Ray* r, 
  BoundingObj::unit_t& timeHit, BoundingObj::vec3_t& normAxis, BoundingObj::vec3_t& parallelAxis){
  BoundingObj::vec3_t ax = b->getAx(), 
    ay = b->getAy(),
    az = b->getAz();
  BoundingObj::unit_t hw = b->getHw(),
    hh = b->getHh(),
    hd = b->getHd();

  float t = 0;
  float tfirst = 0.0f, tlast = 1.0f;

  //convert origin to vector3
  BoundingObj::vec3_t origin(r->getOrigin().x,r->getOrigin().y,r->getOrigin().z);
  BoundingObj::vec3_t centerBox(b->getCenter().x, b->getCenter().y, b->getCenter().z);

  BoundingObj::unit_t oldtfirst = tfirst;
  normAxis = ax;
  parallelAxis = ay;

  if (!raySlab(origin.dot(ax), r->getDirection().dot(ax), 
    centerBox.dot(ax) - hw, centerBox.dot(ax) + hw, tfirst, tlast)) 
    return std::pair<bool,BoundingObj::vec3_t>(false,BoundingObj::vec3_t());

  if (!raySlab(origin.dot(ay), r->getDirection().dot(ay), 
    centerBox.dot(ay) - hh, centerBox.dot(ay) + hh, tfirst, tlast))
    return std::pair<bool,BoundingObj::vec3_t>(false,BoundingObj::vec3_t());

  if(oldtfirst != tfirst){
    oldtfirst = tfirst;
    normAxis = ay;
    parallelAxis = ax;
  }

  if (!raySlab(origin.dot(az), r->getDirection().dot(az), 
    centerBox.dot(az) - hd, centerBox.dot(az) + hd, tfirst, tlast))
    return std::pair<bool,BoundingObj::vec3_t>(false,BoundingObj::vec3_t());

  if(oldtfirst != tfirst){
    oldtfirst = tfirst;
    normAxis = az;
    parallelAxis = BoundingObj::vec3_t(0,0,0);
  }

  //if tfirst is 0 the normal Axis may be incorrect, this tests each plane
  if(true){
  //if(tfirst < 1.0E-8){
  BoundingObj::vec3_t d = r->getDirection();
  d.scale(tfirst);
  BoundingObj::vec3_t testPoint = origin + d; //intersection point

  BoundingObj::vec3_t axis = ax;
  int i = 0;
  for( i = 0; i < 3; i++){
    switch(i){
    case 0:
      axis = ax;
      axis.scale(hw);
      break;
    case 1:
      axis = ay;
      axis.scale(hh);
      break;
    case 2:
      axis = az;
      axis.scale(hd);
      break;
    }

    BoundingObj::vec3_t planePoint = centerBox + axis;
    BoundingObj::vec3_t planePoint2 = centerBox - axis;
    BoundingObj::vec3_t v = testPoint - planePoint;
    BoundingObj::vec3_t v2 = testPoint - planePoint2;
    if( fabs(v.dot(axis)) < 1.0E-8 || fabs(v2.dot(axis)) < 1.0E-8){
      break;
    }
  }

  switch(i){
  case 0:
    normAxis = ax;
    parallelAxis = ay;
    break;
  case 1:
    normAxis = ay;
    parallelAxis = ax;
    break;
  case 2:
    normAxis = az;
    parallelAxis = BoundingObj::vec3_t(0,0,0);
    break;
  }
  }

  //the time it hit
  t = tfirst;
  timeHit = t;
  BoundingObj::vec3_t ret = r->getDirection();
  ret.scale(1.0f-t);
  //ret.scale(1.0-tfirst);
  //ret.negate();

  return std::pair<bool,BoundingObj::vec3_t>(true,ret);
}

std::pair<bool,BoundingObj::vec3_t> sphereSphere(const BoundingSphere* o, const BoundingSphere* t){
  BoundingObj::vec3_t v3 = o->getCenter() - t->getCenter();

  //TODO fix this, if the centers are the same how to displace?
  if(v3.x == 0 && v3.y == 0 && v3.z == 0){
    v3 = BoundingObj::vec3_t(1,0,0);
  }
  
  BoundingObj::unit_t mag = v3.magnitude();
  BoundingObj::unit_t radSum = o->getRadius() + t->getRadius();
  v3.normalize();
  v3.scale(radSum - mag);

  return std::pair<bool, BoundingObj::vec3_t>( mag < radSum, v3);
}

std::pair<bool,BoundingObj::vec3_t> sphereRay(const BoundingSphere* s, const Ray* r){
  //project the center onto the line of the ray
  float ab2 = r->getDirection().dot(r->getDirection());
  BoundingObj::vec3_t ap = s->getCenter() - r->getOrigin();
  float ap_dot_ab = ap.dot(r->getDirection());
  float t = ap_dot_ab / ab2;

  if( t < 0.0f ) t = 0.0f;
  else if( t > 1.0f ) t = 1.0f;

  //q is closest point
  BoundingObj::vec4_t q = r->getOrigin() + r->getDirection() * t;
  
  //check if that point is in the circle
  BoundingObj::vec3_t pq = q - s->getCenter();
  float pq2 = pq.dot(pq);
  float r2 = s->getRadius() * s->getRadius();

  //calculate return vec
  BoundingObj::vec3_t ret = r->getDirection();
  ret.scale(1.0f-t);
  //ret.scale(1.0-t);
  //ret.negate();
  return std::pair<bool, BoundingObj::vec3_t>(pq2<=r2, ret);
}

std::pair<bool,BoundingObj::vec3_t> collide(const BoundingObj * a,const  BoundingObj * b){
  if(a->isBox() && b->isBox())
    return boxBox((const BoundingBox*)a,(const BoundingBox*)b);
  else if(a->isBox() && b->isRay())
    return boxRay((const BoundingBox*)a,(const Ray*)b);
  else if(b->isBox() && a->isRay()){
    std::pair<bool,BoundingObj::vec3_t> r = boxRay((const BoundingBox*)b,(const Ray*)a);
    r.second.negate();
    return r;
  }
  
  else if(a->isSphere() && b->isSphere())
    return sphereSphere((const BoundingSphere*)a,(const BoundingSphere*)b);
  else if(a->isSphere() && b->isRay())
    return sphereRay((const BoundingSphere*)a,(const Ray*)b);
  else if(b->isSphere() && a->isRay()){
    std::pair<bool,BoundingObj::vec3_t> r = sphereRay((const BoundingSphere*)b,(const Ray*)a);
    r.second.negate();
    return r;
  }

  return std::pair<bool,BoundingObj::vec3_t>(false,BoundingObj::vec3_t());
}

RayCollision rayCollide(const Ray * r,const  BoundingObj * b){
  RayCollision res;

  if(b->isBox()){ //TODO right now this only works for boxes
    std::pair<bool,BoundingObj::vec3_t> res2 = 
      boxRay((const BoundingBox *)b, r, res.tfirst, res.normalAxis, res.parallelAxis);
    res.collided = res2.first;
  }

  return res;
}

void boxTest(){
  /*
  //r1 and b1 bad norm
    Ray r1(BoundingObj::vec4_t(-0.283931f, 9, 2.91043f), 
      BoundingObj::vec3_t(0.339934f, 0.999422f, 0));
    
    BoundingBox b1(BoundingObj::vec4_t(-5,10,2), 
      BoundingObj::vec3_t(1,0,0), BoundingObj::vec3_t(0,1,0), BoundingObj::vec3_t(0,0 ,1),
      5,1,2);

    //r1 and b3 bad norm
    Ray r2(BoundingObj::vec4_t(-11, -3.88816, 2.89663f), 
      BoundingObj::vec3_t(0.999986f, -0.00520365f, 0));

    BoundingBox b3(BoundingObj::vec4_t(-10,-5,2), 
      BoundingObj::vec3_t(0,-1,0), BoundingObj::vec3_t(1,0,0), BoundingObj::vec3_t(0,0 ,1),
      5,1,2);
    */

  /*
    //floor
    BoundingBox b2(BoundingObj::vec4_t(0,0,-5), 
      BoundingObj::vec3_t(0,-1,0), BoundingObj::vec3_t(1,0,0), BoundingObj::vec3_t(0,0 ,1),
      500,500,5);
    
    BoundingBox player(BoundingObj::vec4_t(12.9452, -9.96372, 2.98109), 
      //BoundingObj::vec3_t(1, 0, 0), BoundingObj::vec3_t(0, 1,0), BoundingObj::vec3_t(0,0 ,1),
      BoundingObj::vec3_t(0.980067, -0.198669, 0), BoundingObj::vec3_t(0.198669, 0.980067,0), BoundingObj::vec3_t(0,0 ,1),
      1,1,3);

    std::cout << "1==" << collide(&player,&b2).first << " vecfix " << collide(&player,&b2).second << std::endl;
    player.setCenter( player.getCenter() + collide(&player,&b2).second );
        std::cout << "1==" << collide(&player,&b2).first << " vecfix " << collide(&player,&b2).second << std::endl;

        */
    /*
    RayCollision f = rayCollide(&r1, &b1);
    std::cout << f.collided << std::endl
      << f.e << std::endl 
      << "norm " << f.normalAxis << std::endl
      << "parallel " << f.parallelAxis << std::endl
      << f.tfirst << std::endl;
      */
  /*

  BoundingBox b2(BoundingObj::vec4_t(1,0,0), 
    BoundingObj::vec3_t(1,0,0), BoundingObj::vec3_t(0,1,0), BoundingObj::vec3_t(0,0,1),
    5,5,5);
    
  BoundingBox b3(BoundingObj::vec4_t(0,0,0), 
    BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
    5,5,5);
  BoundingBox b4(BoundingObj::vec4_t(7,0,0), 
    BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
    5,5,5);
  BoundingBox b5(BoundingObj::vec4_t(14.3f,0,0), 
    BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
    5,5,5);
  std::cout << "1==" << collide(&b1,&b2).first << collide(&b2,&b1).first << std::endl;
    std::cout << "" << collide(&b1,&b2).second << collide(&b2,&b1).second << std::endl;
  std::cout << "1==" << collide(&b3,&b4).first << collide(&b4,&b3).first << std::endl;
    std::cout << "" << collide(&b3,&b4).second << collide(&b4,&b3).second << std::endl;
  std::cout << "0==" << collide(&b3,&b5).first << collide(&b5,&b3).first << std::endl;
    std::cout << "" << collide(&b3,&b5).second << collide(&b5,&b3).second << std::endl;

  BoundingBox b6(BoundingObj::vec4_t(0,0,-1000), 
    BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
    1000,1000,1000);
  BoundingBox b7(BoundingObj::vec4_t(0,0,0), 
    BoundingObj::vec3_t(1,1,0), BoundingObj::vec3_t(1,-1,0), BoundingObj::vec3_t(0,0,1),
    5,5,5);
  std::cout << "asdf1=" << collide(&b6,&b7).first << collide(&b7,&b6).first << std::endl;
  std::cout << "" << collide(&b6,&b7).second << collide(&b7,&b6).second << std::endl;

  Ray r1(BoundingObj::vec4_t(-50,0,0), BoundingObj::vec3_t(100,0,0));
  Ray r2(BoundingObj::vec4_t(0,0,0), BoundingObj::vec3_t(1000,1000,0));
  std::cout << "1==" << collide(&b1,&r1).first << collide(&r1,&b1).first << std::endl;
    std::cout << "" << collide(&b1,&r1).second << collide(&r1,&b1).second << std::endl;
  std::cout << "1==" << collide(&b3,&r1).first << collide(&r1,&b3).first << std::endl;
    std::cout << "" << collide(&b3,&r1).second << collide(&r1,&b3).second << std::endl;
  std::cout << "0==" << collide(&b5,&r2).first << collide(&b5,&r2).first << std::endl;
    std::cout << "" << collide(&b5,&r2).second << collide(&b5,&r2).second << std::endl;
    */
}

void sphereTest(){
  BoundingSphere s1(BoundingObj::vec4_t(0,0,0),100);
  BoundingSphere s2(BoundingObj::vec4_t(109,0,0),10);
  BoundingSphere s3(BoundingObj::vec4_t(0,10,0),10);
  std::cout << "true 1==" << collide(&s1, &s2).first << collide(&s2, &s1).first << std::endl;
    std::cout << "" << collide(&s1, &s2).second << collide(&s2, &s1).second << std::endl;
  std::cout << "true 1==" << collide(&s1, &s3).first << collide(&s3, &s1).first << std::endl;
    std::cout << "" << collide(&s1, &s3).second << collide(&s3, &s1).second << std::endl;
  std::cout << "false 0==" << collide(&s3, &s2).first << collide(&s2, &s3).first << std::endl;
    std::cout << "" << collide(&s3, &s2).second << collide(&s2, &s3).second << std::endl;

  Ray r1(BoundingObj::vec4_t(109,0,0), BoundingObj::vec3_t(-10,0,0));
  Ray r2(BoundingObj::vec4_t(0,40,0), BoundingObj::vec3_t(0,-1000,0));
  Ray r3(BoundingObj::vec4_t(0,40,0), BoundingObj::vec3_t(0,1000,0));
  std::cout << "true 1==" << collide(&s1, &r1).first << collide(&r1, &s1).first << std::endl;
    std::cout << "" << collide(&s1, &r1).second << collide(&r1, &s1).second << std::endl;
  std::cout << "true 1==" << collide(&s3, &r2).first << collide(&r2, &s3).first << std::endl;
     std::cout << "" << collide(&s3, &r2).second << collide(&r2, &s3).second << std::endl;
  std::cout << "false 0==" << collide(&s3, &r3).first << collide(&r3, &s3).first << std::endl;
    std::cout << "" << collide(&s3, &r3).second << collide(&r3, &s3).second << std::endl;
}

