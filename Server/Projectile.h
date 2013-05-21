#pragma once
#include "DeadlyEntity.h"
#include "Player.h"
class Projectile :

public DeadlyEntity
{
public:
  static const Entity_Type type =PROJECTILE;
  Projectile():fired(false) {/*TODO initialize */ }
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
  void handleCollisions();
  void clearEvents();
  Entity_Type getType() const {
    return type;
  }
  void serialize(sf::Packet & packet) const;
  void deserialize(sf::Packet & packet);
 
private:
  Player * owner;
  bool fired;
  bool correctMovementHit( Entity* e );

public:
  bool getFired() const{ return fired; }
  void setFired(bool f) { fired = f; }
};

