#pragma once
#include "DeadlyEntity.h"
#include "Player.h"

struct ProjectileData{
  MAGIC_POWER magicType;
  length_t range;
  length_t speed;
  float strength;
  int chargeTime;

  ProjectileData( MAGIC_POWER magicType1, 
    length_t range1,
    length_t speed1,
    float strength1,
    int chargeTime1)
    : magicType(magicType), range(range1), speed(speed1), 
    strength(strength1), chargeTime(chargeTime1){}
};

const ProjectileData ProjInfo[] = {
  ProjectileData(FIRE1, 300, 100, 26, 1500)
};

class Projectile :
public DeadlyEntity
{
public:
  static const Entity_Type type = PROJECTILE;
  static MAGIC_POWER upgrade( const MAGIC_POWER m );
  static MAGIC_POWER combine( const MAGIC_POWER m1, const MAGIC_POWER m2 );

  length_t range;
  length_t distanceLeftToTravel;

  Projectile():fired(false) {/*TODO initialize */ }
  Projectile(Map* m);
  ~Projectile(void);
  void update(void);
  void updateBounds();
  void updateBoundsSoft();
  void handleCollisions();
  void clearEvents();
  void fire( v3_t velocity);
 
private:
  Player * owner;
  bool fired;
  bool correctMovementHit( Entity* e );
  bool charging;
  MAGIC_POWER magicType;
  sf::Clock charge_counter;
  int Charge_Time; 

public:
  bool getFired() const { return fired; }
  void setFired(bool f) { fired = f; }
  float getStrength();
  void setStrength(float a);
  void setRange(length_t r);
  void setOwner(Player *);
  MAGIC_POWER getMagicType(){ return magicType; }
  void setMagicType( MAGIC_POWER m ) { magicType = m; }
  Player* getOwner(){return owner;}
  void setChargeTime(int t) { Charge_Time = t ;};
  Entity_Type getType() const { return type; }
  void serialize(sf::Packet & packet) const;
  void deserialize(sf::Packet & packet);
};

