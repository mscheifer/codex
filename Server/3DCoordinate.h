#pragma once
struct Coordinate
{
  int x;
  int y;
  int z;
  float velocityX;
  float velocityY;
  float velocityZ;
  Coordinate():x(0),y(0),z(0),velocityX(0),velocityY(0),velocityZ(0) {}
  Coordinate(int a, int b, int c, float va, float vb, float vc) :
    x(a), y(b), z(c), velocityX(va), velocityY(vb), velocityZ(vc){}
};