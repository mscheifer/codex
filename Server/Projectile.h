#pragma once
#include "DeadlyEntity.h"
#include "Player.h"

struct ProjectileData{
  MAGIC_POWER magicType;
  length_t range;
  length_t speed;
  float strength;
  float mpCost;
  int chargeTime; //charge time in milliseconds
  BUFF debuff;

  ProjectileData( MAGIC_POWER magicType1, 
    length_t range1,
    length_t speed1,
    float strength1,
    float mpCost1,
    BUFF debuff1,
    int chargeTime1
    )
    : magicType(magicType), range(range1), speed(speed1), 
    strength(strength1), mpCost(mpCost1), chargeTime(chargeTime1), debuff(debuff1){}
};

const ProjectileData ProjInfo[] = { //TODO fix all these numbers
  //             type,  rng, spd str mp buff chrgTime
  ProjectileData(FIR1,  300, 30, 10, 2, FIR1DEBUFF, -1),
  ProjectileData(FIR2,  300, 30, 12, 0, NONE, 5000),
  ProjectileData(FIR3,  300, 30, 20, 0, NONE, -1),
  ProjectileData(ICE1,  300, 30, 10, 0, NONE, 1500),
  ProjectileData(ICE2,  300, 30, 12, 0, NONE, 5000),
  ProjectileData(ICE3,  300, 30, 20, 0, NONE, -1),
  ProjectileData(THU1,  300, 30, 10, 0, NONE, 1500),
  ProjectileData(THU2,  300, 30, 12, 0, NONE, 5000),
  ProjectileData(THU3,  300, 30, 20, 0, NONE, -1),
  ProjectileData(G_IT,  300, 30, 12, 0, NONE, -1),
  ProjectileData(G_FT,  300, 30, 12, 0, NONE, -1),
  ProjectileData(G_FI,  300, 30, 12, 0, NONE, -1),
  ProjectileData(G2,    300, 30, 25, 0, NONE, -1),
  ProjectileData(G_IT2, 300, 30, 20, 0, NONE, -1),
  ProjectileData(G_FT2, 300, 30, 20, 0, NONE, -1),
  ProjectileData(G_FI2, 300, 30, 20, 0, NONE, -1),
  ProjectileData(G3,    300, 30, 50, 0, NONE, -1),
  ProjectileData(B1,    50,  30, 5, 0, NONE, -1)
};

class Projectile :
public DeadlyEntity
{
public:
  static const Entity_Type type = PROJECTILE;
  static MAGIC_POWER upgrade( const MAGIC_POWER m );
  static MAGIC_POWER combine( MAGIC_POWER m1, MAGIC_POWER m2 );
  static const MAGIC_POWER combinations[18][18];

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
  void fire(v3_t velocity, float strengthMultiplier);
  std::string toString();
  void reset();
 
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
  void setMagicType( MAGIC_POWER m ) { magicType = m; charge_counter.restart(); }
  Player* getOwner(){return owner;}
  void setChargeTime(int t) { Charge_Time = t ;};
  Entity_Type getType() const { return type; }
  void serialize(sf::Packet & packet) const;
  void deserialize(sf::Packet & packet);
};

