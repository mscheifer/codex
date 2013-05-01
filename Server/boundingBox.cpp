#include "boundingBox.h"

bool BoundingBox::separatedByAxis(const gx::vector3 t, const gx::vector3 axis, const BoundingBox & b){
  float centerDist = abs(t.dot(axis));
  float sumDist = 0;

  //Ax*Aw dot L
  gx::vector3 axis2 = ax;
  axis2.scale(hw);
  sumDist += abs(axis2.dot(axis));

  //Ay*Ah dot L
  axis2 = ay;
  axis2.scale(hh);
  sumDist += abs(axis2.dot(axis));

  //Az*Ad dot L
  axis2 = az;
  axis2.scale(hd);
  sumDist += abs(axis2.dot(axis));

  //Bx*Bw dot L
  axis2 = b.ax;
  axis2.scale(b.hw);
  sumDist += abs(axis2.dot(axis));

  //By*Bh dot L
  axis2 = b.ay;
  axis2.scale(b.hh);
  sumDist += abs(axis2.dot(axis));

  //Bz*Bd dot L
  axis2 = b.az;
  axis2.scale(b.hd);
  sumDist += abs(axis2.dot(axis));

  return centerDist > sumDist;
}

bool BoundingBox::collideWith(const BoundingBox & b){
  //the vector between the centers
  gx::vector3 t = b.center - center;
  gx::vector3 axbx, axby, axbz, aybx, ayby, aybz, azbx, azby, azbz;
  axbx.cross(ax,b.ax);
  axby.cross(ax,b.ay);
  axbz.cross(ax,b.az);
  
  aybx.cross(ay,b.ax);
  ayby.cross(ay,b.ay);
  aybz.cross(ay,b.az);
  
  azbx.cross(az,b.ax);
  azby.cross(az,b.ay);
  azbz.cross(az,b.az);

  /*
  std::cout << !separatedByAxis(t,ax,b);
  std::cout << !separatedByAxis(t,ay,b); 
  std::cout << !separatedByAxis(t,az,b); 
  std::cout << !separatedByAxis(t,b.ax,b); 
  std::cout << !separatedByAxis(t,b.ay,b); 
  std::cout << !separatedByAxis(t,b.az,b); 
  std::cout << !separatedByAxis(t,axbx,b); 
  std::cout << !separatedByAxis(t,axby,b); 
  std::cout << !separatedByAxis(t,axbz,b); 
  std::cout << !separatedByAxis(t,aybx,b);  
  std::cout << !separatedByAxis(t,ayby,b);  
  std::cout << !separatedByAxis(t,aybz,b); 
  std::cout << !separatedByAxis(t,azbx,b);  
  std::cout << !separatedByAxis(t,azby,b);
  std::cout << !separatedByAxis(t,azbz,b) << std::endl;
  */

  return !separatedByAxis(t,ax,b) && !separatedByAxis(t,ay,b) && !separatedByAxis(t,az,b) &&
    !separatedByAxis(t,b.ax,b) && !separatedByAxis(t,b.ay,b) && !separatedByAxis(t,b.az,b) &&
    !separatedByAxis(t,axbx,b) && !separatedByAxis(t,axby,b) && !separatedByAxis(t,axbz,b) &&
    !separatedByAxis(t,aybx,b) && !separatedByAxis(t,ayby,b) && !separatedByAxis(t,aybz,b) &&
    !separatedByAxis(t,azbx,b) && !separatedByAxis(t,azby,b) && !separatedByAxis(t,azbz,b);
}

bool BoundingBox::raySlab(float start, float dir, float min, float max, float& tfirst, float& tlast){
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

bool BoundingBox::collideWith(const Ray & r){
  float t = 0;
  float tfirst = 0.0f, tlast = 1.0f;

  gx::vector3 origin(r.getOrigin().x,r.getOrigin().y,r.getOrigin().z);
  gx::vector3 centerBox(center.x, center.y, center.z);

  if (!raySlab(origin.dot(ax), r.getDirection().dot(ax), 
    centerBox.dot(ax) - hw, centerBox.dot(ax) + hw, tfirst, tlast)) return false;
  if (!raySlab(origin.dot(ay), r.getDirection().dot(ay), 
    centerBox.dot(ay) - hh, centerBox.dot(ay) + hh, tfirst, tlast)) return false;
  if (!raySlab(origin.dot(az), r.getDirection().dot(az), 
    centerBox.dot(az) - hd, centerBox.dot(az) + hd, tfirst, tlast)) return false;

  t = tfirst;
  return true;
}

