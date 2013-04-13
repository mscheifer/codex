#pragma once
struct Coordinate
{
  int x;
  int y;
  int z;
  float velocityX;
  float velocityY;
  float velocityZ;
  Coordinate(int a, int b, int c, int va, int vb, int vc) :
    x(a), y(b), z(c), velocityX(va), velocityY(vb), velocityZ(vc){}
};