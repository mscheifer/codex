#pragma once
#include "3DCoordinate.h"
#include "directionalVector.h"
#include "ClientGameTimeAction.h"
class Entity{
public:
  Entity() {}
  ~Entity() {}
  
  virtual void handleAction(ClientGameTimeAction a){}
  Coordinate getPosition(void){ return position; }
  Direction getDirection(void){ return direction; }


protected:
  Coordinate position;
  Direction direction;
  // Some kind of state {paralyzed, frozen, blah blah}
  // Power ups {contains MULTIPLERS for health, defense/ elemental attributes}
};