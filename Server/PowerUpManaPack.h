#pragma once
#include "PowerUp.h"
class PowerUpManaPack :
  public PowerUp
{
public:
  PowerUpManaPack(void);
  PowerUpManaPack(v3_t p, Map* m);
  ~PowerUpManaPack(void);
};

