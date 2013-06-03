#ifndef GROUND_H
#define GROUND_H
#include "Entity.h"

class Ground : public Entity
{
public:
  static const Entity_Type type = GROUND;

  virtual Entity_Type getType() const { return type; }
}

#endif