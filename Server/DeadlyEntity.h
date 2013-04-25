#pragma once
#include "Entity.h"
class DeadlyEntity :
  public Entity
{
public:
  DeadlyEntity(void);
  ~DeadlyEntity(void);
  float getStrength(void){ return strength; }
protected:
	float range;
	float strength;
};

