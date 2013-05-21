#pragma once
#include "powerup.h"
class PowerUpSpeedPack :
  public PowerUp
{
public:
  PowerUpSpeedPack(void);
  PowerUpSpeedPack(v3_t p, Map* m);
  ~PowerUpSpeedPack(void);
};

