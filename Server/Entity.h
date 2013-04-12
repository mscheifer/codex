#pragma once
#include "3DCoordinate.h"
class Entity{
public:
  virtual ~Entity(){}
protected:
  Coordinate position;
};