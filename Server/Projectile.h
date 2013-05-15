#pragma once
#include "DeadlyEntity.h"
#include "Player.h"
class Projectile :

public DeadlyEntity
{
public:
  static const Entity_Type type =PROJECTILE;
  Projectile() {/*TODO initialize */ }
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
  Entity_Type getType() const {
    return type;
  }
  void serialize(sf::Packet & packet) const {
    Entity::serialize(packet);
    //(*owner).serialize(packet);
  } 
  void deserialize( sf::Packet & packet ) {
    Entity::deserialize(packet);
    //delete owner; this segfaults
    //Player* owner = new Player();
    //(*owner).deserialize(packet);
  }
private:
  Player * owner;
};

