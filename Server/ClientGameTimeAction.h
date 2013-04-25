#pragma once
#include "directionalVector.h"
#include "StaticEnums.h"
#include <iostream>
#include "SFML/Network/Packet.hpp"

struct ClientGameTimeAction
{
  static const int packetType = CGTA;  //might be a BUG

	int player_id;
	User_Movement movement;
	bool attackMelee; // Can use ranged weapons to melee attack
  bool attackRange;
  bool weapon1; // Used for switching weapon
  bool weapon2; // Used for switching weapon
	bool jump;
	Direction facingDirection;

  void clear(){
    movement = NONE;
    attackMelee = attackRange = weapon1 = weapon2 = jump = false;
    facingDirection.x = facingDirection.z = 0;
    facingDirection.y = -1;
  }

	ClientGameTimeAction() : 
    player_id(-1), weapon1(false), weapon2(false),
    attackMelee(false),attackRange(false),
    jump(false), facingDirection(), movement(NONE){};

  void serialize(sf::Packet & packet){
    packet << player_id;
    packet << (int)movement;
    packet << attackMelee;
    packet << weapon1;
    packet << weapon2;
    packet << jump;
    facingDirection.serialize(packet);
  }

  void deserialize(sf::Packet & packet){
    packet >> player_id;
    int movementInt = 0;
    packet >> movementInt;
    movement = (User_Movement)movementInt;
    packet >> attackMelee;
    packet >> weapon1;
    packet >> weapon2;
    packet >> jump;
    facingDirection.deserialize(packet);
  }

  void print(){
    std::cout << "id " << player_id << std::endl
      << "mov " << movement << std::endl
      << "attackMelee " << attackMelee << std::endl
      << "weapon1 " << weapon1 << std::endl
      << "weapon2 " << weapon2 << std::endl
      << "jump " << jump << std::endl
      << "facingDirection " << facingDirection.x << " "<< facingDirection.y << " " << facingDirection.z << std::endl; 
  }
};
