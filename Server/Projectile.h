#pragma once
#include "deadlyentity.h"
#include "Player.h"
class Projectile :
  public DeadlyEntity
{
public:
  Projectile(void);
  ~Projectile(void);
private:
  Player * owner;
};

