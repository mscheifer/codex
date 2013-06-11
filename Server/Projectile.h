#pragma once
#include "DeadlyEntity.h"
#include "Player.h"

struct ProjectileData{
  MAGIC_POWER magicType;
  int level;
  length_t range;
  length_t speed;
  length_t size;
  float strength;
  float mpCost;
  int chargeTime; //charge time in milliseconds
  std::vector<BUFF> debuff;

  ProjectileData( MAGIC_POWER magicType1, 
    int level1,
    length_t range1,
    length_t speed1,
    length_t size1,
    float strength1,
    float mpCost1,
    std::vector<BUFF> debuff1,
    int chargeTime1
    )
    : magicType(magicType1), level(level1), range(range1), speed(speed1), size(size1),
    strength(strength1), mpCost(mpCost1), chargeTime(chargeTime1), debuff(debuff1){}
};

const BUFF FIR1arr[] = {FIR1DEBUFF};
const BUFF FIR2arr[] = {FIR2DEBUFF};
const BUFF FIR3arr[] = {FIR3DEBUFF};
const BUFF ICE1arr[] = {ICE1DEBUFF};
const BUFF ICE2arr[] = {ICE2DEBUFF};
const BUFF ICE3arr[] = {ICE3DEBUFF, ICESTUN};
const BUFF THU1arr[] = {THU1DEBUFF};
const BUFF THU2arr[] = {THU2DEBUFF, THUSTUN};
const BUFF THU3arr[] = {THU3DEBUFF, THUSTUN2};
const BUFF G1Marr[] = {G1MDEBUFF};
const BUFF G2arr[] = {G2DEBUFF, THUSTUN};
const BUFF G2Marr[] = {G2MDEBUFF, THUSTUN};
const BUFF G3arr[] = {G3DEBUFF1, G3DEBUFF2};

const BUFF NONEarr[] = {NONE};

//the size is harded coded right now lol 
const std::vector<BUFF> FIR1V(FIR1arr,FIR1arr+1);
const std::vector<BUFF> FIR2V(FIR2arr,FIR2arr+1);
const std::vector<BUFF> FIR3V(FIR3arr,FIR3arr+1);
const std::vector<BUFF> ICE1V(ICE1arr,ICE1arr+1);
const std::vector<BUFF> ICE2V(ICE2arr,ICE2arr+1);
const std::vector<BUFF> ICE3V(ICE3arr,ICE3arr+2);
const std::vector<BUFF> THU1V(THU1arr,THU1arr+1);
const std::vector<BUFF> THU2V(THU2arr,THU2arr+2);
const std::vector<BUFF> THU3V(THU3arr,THU3arr+2);
const std::vector<BUFF> G1MV(G1Marr,G1Marr+1);
const std::vector<BUFF> G2V(G2arr,G2arr+1);
const std::vector<BUFF> G2MV(G2Marr,G2Marr+1);
const std::vector<BUFF> G3V(G3arr,G3arr+2);

const std::vector<BUFF> NONEV(NONEarr,NONEarr+1);

//these need to match the MAGIC_POWER enum, the type is just for readability
const ProjectileData ProjInfo[] = { //TODO fix all these numbers
  //             type,  lvl, rng, spd size    str mp buff chrgTime
  ProjectileData(FIR1,  1,   300, 400, 1.5,   10, 10, NONEV, 5000),
  ProjectileData(FIR2,  2,   300, 400, 2.25,  13, 20,  FIR2V, -1),//7000),
  ProjectileData(FIR3,  3,   300, 400, 3,     20, 30,  FIR3V, -1),
  ProjectileData(ICE1,  1,   300, 400, 1.5,   10, 10, NONEV, 5000),
  ProjectileData(ICE2,  2,   300, 400, 2.25,  13, 20,  ICE2V, -1),//7000),
  ProjectileData(ICE3,  3,   300, 400, 3,     20, 30,  ICE3V, -1),
  ProjectileData(THU1,  1,   300, 400, 1.5,   10, 10, NONEV, 5000),
  ProjectileData(THU2,  2,   300, 400, 2.25,  13, 20,  THU2V, -1),//7000),
  ProjectileData(THU3,  3,   300, 400, 3,     20, 30,  THU3V, -1),

  ProjectileData(G_IT,  1,   300, 400, 1,   16, 10, G1MV, -1),
  ProjectileData(G_FT,  1,   300, 400, 1,   16, 10, G1MV, -1),
  ProjectileData(G_FI,  1,   300, 400, 1,   16, 10, G1MV, -1),

  ProjectileData(G2,    3,   300, 400, 3,   25, 20, G2V, -1),

  ProjectileData(G_IT2, 2,   300, 400, 2,   20, 20, G2MV, -1),
  ProjectileData(G_FT2, 2,   300, 400, 2,   20, 20, G2MV, -1),
  ProjectileData(G_FI2, 2,   300, 400, 2,   20, 20, G2MV, -1),

  ProjectileData(G3,    3,   300, 400, 3,   50, 30, G3V, -1),

  ProjectileData(B1,    1,   100, 400, 1,   7,  5, NONEV, 5000),
  ProjectileData(B2,    1,   300, 400, 2,   10, 5, NONEV, 7000),
  ProjectileData(B2,    3,   300, 400, 3,   10, 10, NONEV, -1)
};

class Projectile :
public DeadlyEntity
{
public:
  static const Entity_Type type = PROJECTILE;
  static MAGIC_POWER upgrade( const MAGIC_POWER m );
  static MAGIC_POWER combine( MAGIC_POWER m1, MAGIC_POWER m2 );
  static const MAGIC_POWER combinations[20][20];
  static int ID_Counter;
  static const float meleeWidth;
  static const float meleeHeight;
  static const float meleeDepth;
  static const float projWidth;
  static const float projHeight;
  static const float projDepth;

  bool live; //if proj is in air or not
  bool combined; //for sound event
  length_t range;
  length_t distanceLeftToTravel;

  Projectile():fired(false) {/*TODO initialize */ }
  Projectile(Map* m);
  void update(void);
  void updateBounds();
  void updateBoundsSoft();
  void handleCollisions();
  void clearEvents();
  void fire(v3_t velocity, float strengthMultiplier);
  void fireMutiple(v3_t v, float strengthMultiplier, int number);
  std::string toString();
  void reset();
  bool sameTeam( Projectile * p );
  bool sameTeam( Player *p);
 
private:
  Player * owner;
  bool fired;
  bool charging;
  MAGIC_POWER magicType;
  sf::Clock charge_counter;
  int Charge_Time;

  bool correctMovementHit( Entity* e );


public:
  int id;
  bool getFired() const { return fired; }
  void setFired(bool f) { fired = f; }
  float getStrength();
  void setStrength(float a);
  void setRange(length_t r);
  void setOwner(Player *);
  void setCharing( bool c ){ charging = c; }
  MAGIC_POWER getMagicType() const { return magicType; }
  void setMagicType( MAGIC_POWER m, bool melee = false); //TODO this is not the best way
  Player* getOwner(){return owner;}
  void setChargeTime(int t) { Charge_Time = t ;};
  Entity_Type getType() const { return type; }
  int getElapsedTime() const { return charge_counter.getElapsedTime().asMilliseconds(); }
  void serialize(sf::Packet & packet) const;
  void deserialize(sf::Packet & packet);
  std::string toString2();
};

