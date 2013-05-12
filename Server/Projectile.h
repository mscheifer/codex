#pragma once
#include "DeadlyEntity.h"
#include "Player.h"
class Projectile :

public DeadlyEntity
{
public:
  static const Entity_Type type =PROJECTILE;
  Projectile(Map* m);
  ~Projectile(void);
  void update(void);
  void setStrength(float a);
  void setRange(length_t r);
  length_t range;
  length_t distanceLeftToTravel;
  void setOwner(Player *);
  Player* getOwner(){return owner;}
  void updateBounds();
  void updateBoundsSoft();
  Entity_Type getType() {
    return type;
  }
private:
  Player * owner;
};

