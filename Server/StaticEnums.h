#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

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
enum Opcode {INIT,JOINID, CHAT, T1, T2, CGTA, SGTR, ERROR};

enum Entity_Type {UNDEFINED, PROJECTILE, PLAYER, WEAPON, WALL};
