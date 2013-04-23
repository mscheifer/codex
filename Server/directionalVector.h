#pragma once
typedef float DirectionValue;
struct Direction
{
  DirectionValue x;
  DirectionValue y;
  DirectionValue z;
  Direction() :
    x(0), y(0), z(0){}
  Direction(DirectionValue a, DirectionValue b, DirectionValue c) :
    x(a), y(b), z(c){}
};