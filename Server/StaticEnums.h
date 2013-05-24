#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

//const int NUM_PLAYERS = 2; use the config file now

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

enum WeaponType{UNK=0, FIRE, FIST};

const std::string WeaponNames[] = {
  "UNKNOWN",
  "STAFF OF FIRE",
  "FIST"
};

enum MAGIC_POWER { FIRE1=0, FIRE2, FIRE3, ICE1, ICE2, ICE3 };

const float magic_mutiplier[3] = { 1.0, 1.5, 3.0};

enum BUFF{ NONE=0, MOVEBOOST };

struct BuffData{
  BUFF buff;
  int ticksEffect;
  bool affectMovement;
  float movementMultiplier;
  BuffData( BUFF buff1,
    int ticksEffect1,
    bool affectMovement1,
    float movementMultiplier1)
  : buff(buff1), ticksEffect(ticksEffect1), 
  affectMovement(affectMovement1), movementMultiplier(movementMultiplier1){}
};

const BuffData BuffInfo[] = {
  BuffData(NONE, 100, false, 2),
  BuffData(MOVEBOOST, 100, true, 2)
};
