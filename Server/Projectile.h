#pragma once
#include "deadlyentity.h"
#include "Player.h"
class Projectile :

public DeadlyEntity
{
public:
  Projectile(void);
  Projectile(Position r);
  ~Projectile(void);
  void update(void);
  Position range;
  Position distanceLeftToTravel;
  bool isProjectile(){return true;}

private:
  Player * owner;
};

