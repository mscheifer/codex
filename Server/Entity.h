#pragma once
#include "3DCoordinate.h"
#include "directionalVector.h"
#include "ClientGameTimeAction.h"
class Entity{
public:
  Entity() {}
  ~Entity() {}
  
  virtual void handleAction(ClientGameTimeAction a){}

protected:
  Coordinate position;
  Direction direction;
};