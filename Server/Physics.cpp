#include "Physics.h"

Position calculatePosition(Position x, Velocity v, Acceleration a)
{
  //x = x0 + v0t + ½at2
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

Coordinate ThreeDMovement(Coordinate location, Direction direction, Acceleration accelerationZ)
{
  double length = sqrt(direction.x *direction.x + direction.y * direction.y + direction.z * direction.z);
  Position newX = calculatePosition(location.x, location.velocityX);
  Position newY = calculatePosition(location.y, location.velocityY);
  Position newZ = calculatePosition(location.z, location.velocityZ, accelerationZ/length);
  Velocity newVZ = calculateVelocity( location.velocityZ, direction.z * accelerationZ/length);

  return Coordinate(newX, newY, newZ, location.velocityX, location.velocityY, newVZ);
}
