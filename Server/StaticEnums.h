#pragma once
#include <cmath>
#include <string>

enum move_t {
  FORWARD = 0,
  FORWARD_LEFT,
  LEFT,
  BACKWARD_LEFT,
  BACKWARD,
  BACKWARD_RIGHT,
  RIGHT,
  FORWARD_RIGHT,
  NULL_DIR
};

const double movementAngles[8] = {
  0.0,
  M_PI_4,
  M_PI_2,
  M_PI_2 + M_PI_4,
  M_PI,
  -(M_PI_2 + M_PI_4),
  -M_PI_2,
  -M_PI_4,
};

enum Game_State {MANOTAUR_WIN, CIVILIAN_WIN, PLAYING}; 

//Networking enums
enum Opcode {INIT,JOINID,STARTGAME, CHAT, T1, T2, CGTA, SGTR, ERROR};

enum Entity_Type {UNDEFINED=0, PLAYER, WALL, PROJECTILE, WEAPON, POWER_UP};

//used to determine names
enum WeaponType{UNK=0, FIRE, ICE, THUNDER, FIST, BASIC};

const std::string WeaponNames[] = {
  "UNKNOWN",
  "STAFF OF FIRE",
  "ICE",
  "MJORLNIR",
  "FIST",
  "BASIC"
};

enum MAGIC_POWER { 
  FIR1=0, FIR2, FIR3, 
  ICE1, ICE2, ICE3,
  THU1, THU2, THU3,
  G_IT, G_FT, G_FI, //gravity and what it is missing
  G2,
  G_IT2, G_FT2, G_FI2,
  G3,
  B1
}; 

enum BUFF{ 
  NONE=0, MOVEBOOST, MANABOOST, HEALTHBOOST, STRBOOST, ATTACKCD, CHARGECD,
  FIR1DEBUFF, FIR2DEBUFF, FIR3DEBUFF,
  ICE1DEBUFF, ICE2DEBUFF, ICE3DEBUFF,
  THUSTUN, THUSTUN2,
  THU1DEBUFF, THU2DEBUFF, THU3DEBUFF,
  G1MDEBUFF,
  G2DEBUFF,
  G2MDEBUFF,
  G3DEBUFF1, G3DEBUFF2
  };

//cd less than one means you cd is reduced that much
struct BuffData{
  BUFF buff;
  int code;        //the code for each buff, ones with the same code will use the level to determine which one to put into effect
  int level;       //the level of buff higher will 'override' lower
  int milliEffect; //amount of ticks  that buff will last
  bool affectMovement;
  float movementMultiplier; //how much to multiply movement by ex. 2 will increase speed 2x
  bool affectManaRegen;
  float manaBonus;          //how much percentage to add ex 2 will add 200% -> giving 300% regen
  bool affectHealthRegen;
  float healthBonus;        //same as mana
  bool affectStrength;
  float strengthMultiplier;
  bool affectAttackCD;
  float attackCDMultiplier;
  bool affectChargeCD;
  float chargeCDMult;

  BuffData( BUFF buff1,
    int code1,
    int level1,
    int milliEffect1,
    bool affectMovement1,
    float movementMultiplier1,
    bool affectManaRegen1,
    float manaBonus1,
    bool affectHealthRegen1,
    float healthBonus1,
    bool affectStrength1,
    float strengthMultiplier1,
    bool affectAttackCD1,
    float attackCDMultiplier1,
    bool affectChargeCD1,
    float chargeCDMult1)
  : buff(buff1),
    code(code1),
    level(level1),
    milliEffect(milliEffect1), 
    affectMovement(affectMovement1),
    movementMultiplier(movementMultiplier1),
    affectManaRegen(affectManaRegen1),
    manaBonus(manaBonus1),
    affectHealthRegen(affectHealthRegen1),
    healthBonus(healthBonus1),
    affectStrength(affectStrength1),
    strengthMultiplier(strengthMultiplier1),
    affectAttackCD(affectAttackCD1),
    attackCDMultiplier(attackCDMultiplier1),
    affectChargeCD(affectChargeCD1),
    chargeCDMult(chargeCDMult1){}
};

const BuffData BuffInfo[] = {
  //                  codelvl  time  |  move       |mpRegen      | hpRegen | str           | atkCD      |  chargeCD  
  BuffData(NONE,        0,  1, 100,    false, 0,    false, 0,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(MOVEBOOST,   1,  1, 120000, true, 2,     false, 0,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(MANABOOST,   2,  1, 120000, false, 0,    true, 5,      false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(HEALTHBOOST, 3,  1, 120000, false, 0,    false, 0,     true, 5,     false, 0,    false, 0,    false, 0),
  BuffData(STRBOOST,    4,  1, 120000, false, 0,    false, 0,     false, 0,    true, 2,     false, 0,    false, 0),
  BuffData(ATTACKCD,    5,  1, 120000, false, 0,    false, 0,     false, 0,    false, 0,    true, 0.25,  false, 0),
  BuffData(CHARGECD,    6,  1, 120000, false, 0,    false, 0,     false, 0,    false, 0,    false, 0,    true, 0.25),

  BuffData(FIR1DEBUFF,  7,  1, 1500,   false, 0,    false, 0,     true, -1.5f, false, 0,    false, 0,    false, 0),
  BuffData(FIR2DEBUFF,  7,  2, 1500,   false, 0,    false, 0,     true, -2,    false, 0,    false, 0,    false, 0),
  BuffData(FIR3DEBUFF,  7,  3, 1500,   false, 0,    false, 0,     true, -4,    false, 0,    false, 0,    false, 0),
  
  BuffData(ICE1DEBUFF,  8,  1, 1500,   true, 0.85f, false, 0,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(ICE2DEBUFF,  8,  2, 1500,   true, 0.75f, false, 0,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(ICE3DEBUFF,  8,  3, 1500,   true, 0.65f, false, 0,     false, 0,    false, 0,    false, 0,    false, 0),

  BuffData(THUSTUN,     9,  1, 300,    true, 0,     false, 0,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(THUSTUN2,    9,  2, 400,    true, 0,     false, 0,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(THU1DEBUFF,  10, 1, 1500,   false, 0,    true, -1.5f,  false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(THU2DEBUFF,  10, 2, 1500,   false, 0,    true, -2,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(THU3DEBUFF,  10, 3, 1500,   false, 0,    true, -4,     false, 0,    false, 0,    false, 0,    false, 0),

  //                           time  |  move       |mpRegen      | hpRegen | str           | atkCD      |  chargeCD  
  BuffData(G1MDEBUFF,   11, 1, 1500,   false, 0,    false, 0,     false, 0,    true, .95f,  false, 0,    false, 0),
  BuffData(G2DEBUFF,    11, 2, 1500,   true, 0.85f, true, -2,     true, -1,    true, .90f,  false, 0,    false, 0),
  BuffData(G2MDEBUFF,   11, 1, 1500,   true, 0.95f, true, -1.25f, false, 0,    true, .95f,    false, 0,    false, 0),
  BuffData(G3DEBUFF1,   12, 1, 700,    true, 0,     false, 0,     false, 0,    false, 0,    false, 0,    false, 0),
  BuffData(G3DEBUFF2,   11, 3, 1000,   true, 0.75f, true, -5,     true, -2,    true, .85f,  false, 0,    false, 0)
};
