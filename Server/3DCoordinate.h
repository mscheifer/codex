#pragma once
typedef double Position;
typedef double Velocity;
typedef double Acceleration;
struct Coordinate
{
  Position x;
  Position y;
  Position z;
  Velocity velocityX;
  Velocity velocityY;
  Velocity velocityZ;
  Acceleration MAX_ACCELERATION_X; 
  Acceleration MAX_ACCELERATION_Y;
  Acceleration MAX_ACCELERATION_Z;
  Coordinate():x(0),y(0),z(0),velocityX(0),velocityY(0),velocityZ(0) {}
  Coordinate(Position a, Position b, Position c, Velocity va, Velocity vb, Velocity vc) :
    x(a), y(b), z(c), velocityX(va), velocityY(vb), velocityZ(vc){}
};