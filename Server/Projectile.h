#pragma once
#include "deadlyentity.h"
#include "Player.h"
class Projectile :

public DeadlyEntity
{
public:
  Projectile(void);
  ~Projectile(void);
  bool isProjectile(){return true;}
  void setOwner(Player *);
private:
  Player * owner;
};

