#pragma once
#include "directionalVector.h"

struct ClientGameTimeAction
{
	int player_id;
	bool forward;
	bool back;
	bool left;
	bool right;
	bool attack;
	bool jump;
	Direction facingDirection;


};