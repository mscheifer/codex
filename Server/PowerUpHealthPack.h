#pragma once
#include "PowerUp.h"
class PowerUpHealthPack :
  public PowerUp
{
public:
  PowerUpHealthPack(void){};

  PowerUpHealthPack(v3_t p, Map* m);
  ~PowerUpHealthPack(void);
};

