#pragma once
#include "DeadlyEntity.h"
#include "Player.h"
class Projectile :

public DeadlyEntity
{
public:
  static const Entity_Type type =PROJECTILE;
  Projectile():fired(false),firedGuard(false) {/*TODO initialize */ }
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
  Entity_Type getType() {
    return type;
  }
  void serialize(sf::Packet & packet) const {
    Entity::serialize(packet);
    packet << fired;
    //(*owner).serialize(packet);
  } 
  void deserialize( sf::Packet & packet ) {
    Entity::deserialize(packet);
    packet >> fired;
    //delete owner; this segfaults
    //Player* owner = new Player();
    //(*owner).deserialize(packet);
  }
private:
  Player * owner;
  bool fired;
  bool firedGuard; //because fired will get set to false the frame it spawns

public:
  bool getFired() const{ return fired; }
  void setFired(bool f) { fired = f; }
  bool getFiredGuard() const{ return firedGuard; }
  void setFiredGuard(bool f){ firedGuard = f; }
};

