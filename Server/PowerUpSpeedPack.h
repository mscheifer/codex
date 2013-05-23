#pragma once
#include "PowerUp.h"
class PowerUpSpeedPack :
  public PowerUp
{
public:
  PowerUpSpeedPack(void);
  PowerUpSpeedPack(v3_t p, Map* m);
  ~PowerUpSpeedPack(void);
};

