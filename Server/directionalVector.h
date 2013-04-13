#pragma once
struct Direction
{
  float x;
  float y;
  float z;
  Direction() :
    x(0), y(0), z(0){}
  Direction(float a, float b, float c) :
    x(a), y(b), z(c){}
};