#include "Physics.h"

int calculatePosition(int x, float v, float a)
{
  //x = x0 + v0t + ½at2
  return (int)(x + v * SERVERCLOCK + .5*a*SERVERCLOCK*SERVERCLOCK);
}

int calculatePosition(int x, float v)
{
  //x = x0 + v0t
  return (int)(x + v * SERVERCLOCK);
}

float calculateVelocity(float v, float a)
{
  //v = v0 + at
  return v + a * SERVERCLOCK;
}

Coordinate ThreeDMovement(Coordinate location, Direction direction, float accelerationZ)
{
  float length = sqrt(direction.x *direction.x + direction.y + direction.y + direction.z * direction.z);
  int newX = calculatePosition(location.x, location.velocityX);
  int newY = calculatePosition(location.y, location.velocityY);
  int newZ = calculatePosition(location.z, location.velocityZ, direction.z * accelerationZ/length);
  float newVZ = calculateVelocity( location.velocityZ, direction.z * accelerationZ/length);

  return Coordinate(newX, newY, newZ, location.velocityX, location.velocityY, newVZ);
}
