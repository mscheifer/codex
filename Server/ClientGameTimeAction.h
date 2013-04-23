#pragma once
#include "directionalVector.h"
#include "StaticEnums.h"

struct ClientGameTimeAction
{
	
	int player_id;
	User_Movement movement;
	bool attackMelee; // Can use ranged weapons to melee attack
  bool attackRange;
  bool weapon1; // Used for switching weapon
  bool weapon2; // Used for switching weapon
	bool jump;
	Direction facingDirection;

	ClientGameTimeAction() : 
    player_id(-1), weapon1(false), weapon2(false),
    attackMelee(false),attackRange(false),
    jump(false), facingDirection(), movement(NONE){};
};