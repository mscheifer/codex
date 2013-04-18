#pragma once
#include "directionalVector.h"
#include "StaticEnums.h"

struct ClientGameTimeAction
{
	
	int player_id;
	User_Movement movement;
	bool attack;
	bool jump;
	Direction facingDirection;

	ClientGameTimeAction() : player_id(-1), attack(false), jump(false), facingDirection(), movement(User_Movement::NONE ){};
};