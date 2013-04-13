#pragma once
#include "3DCoordinate.h"
#include "directionalVector.h"
class Entity{
public:
  virtual ~Entity(){}

protected:
	Coordinate position;
  Direction direction;
};