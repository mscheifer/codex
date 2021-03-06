#include "Physics.h"
/*
Position calculatePosition(Position x, Velocity v, Acceleration a)
{
  //x = x0 + v0t + �at2
  return x + v * SERVERCLOCK + .5*a*SERVERCLOCK*SERVERCLOCK;
}

Position calculatePosition(Position x, Velocity v)
{
  //x = x0 + v0t
  return x + v * SERVERCLOCK;
}

Position calculateVelocity(Velocity v, Acceleration a)
{
  //v = v0 + at
  return v + a * SERVERCLOCK;
}

Position calculateDistanceInBetween( Coordinate x, Coordinate y){
	// dist = sqrt( (x-x)^2 + (y-y)^2  + (z-z)^2  )
	return sqrt( pow((x.x - y.x), 2) + pow((x.y - y.y), 2) + pow((x.z - y.z), 2));
}

Coordinate ThreeDMovement(Coordinate location, Direction direction, Acceleration accelerationZ)
{
  double length = sqrt(direction.x *direction.x + direction.y * direction.y + direction.z * direction.z);
  Position newX = calculatePosition(location.x, location.velocityX);
  Position newY = calculatePosition(location.y, location.velocityY);
  Position newZ = calculatePosition(location.z, location.velocityZ, accelerationZ/length);
  Velocity newVZ = calculateVelocity( location.velocityZ, direction.z * accelerationZ/length);

  return Coordinate(newX, newY, newZ, location.velocityX, location.velocityY, newVZ);
}

*/