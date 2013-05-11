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
  void setRange(Position r);
  Position range;
  Position distanceLeftToTravel;
  void setOwner(Player *);
  Player* getOwner(){return owner;}
  void updateBounds();
  void updateBoundsSoft();
  void handleCollisions();
  Entity_Type getType() {
    return type;
  }
private:
  Player * owner;
};

