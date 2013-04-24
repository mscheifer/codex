#pragma once
enum User_Movement {
		FORWARD=0,
		BACKWARD=180,
		LEFT=-90,
		RIGHT=90,
		FORWARD_LEFT=-45,
		FORWARD_RIGHT=45,
		BACKWARD_LEFT=-135,
		BACKWARD_RIGHT=135,
		NONE
};

//Networking enums
enum Opcode {INIT, CHAT , T1, T2, CGTA};