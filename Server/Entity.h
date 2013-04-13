#pragma once
#include "3DCoordinate.h"
#include "directionalVector.h"
class Entity{
public:
  Entity() {}
  ~Entity() {}

protected:
  Coordinate position;
  Direction direction;
};