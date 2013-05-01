#pragma once
#include "DeadlyEntity.h"
#include "Player.h"
class Projectile :

public DeadlyEntity
{
public:
  Projectile(void);
  Projectile(Position r);
  ~Projectile(void);
  void update(void);
  void setStrength(float a);
  void setRange(Position r);
  Position range;
  Position distanceLeftToTravel;
  bool isProjectile(){return true;}
  void setOwner(Player *);
private:
  Player * owner;
};

