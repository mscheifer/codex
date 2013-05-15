#pragma once
#include <SFML/Network.hpp>
#include "StaticEnums.h"
#include "Physics.h"

struct ClientGameTimeAction
{
  static const int packetType = CGTA;  //might be a BUG
	int player_id;
	move_t movement;
	bool attackMelee; // Can use ranged weapons to melee attack
  bool attackRange;
  bool weapon1; // Used for switching weapon
  bool weapon2; // Used for switching weapon
	bool jump;
  bool updated;
  bool pickup;
	v3_t facingDirection;
  
  bool operator==(const ClientGameTimeAction & other) const;
  bool operator!=(const ClientGameTimeAction & other) const;

  void clear();

	ClientGameTimeAction() : 
    player_id(-1), movement(NULL_DIR),
    attackMelee(false),attackRange(false),
    weapon1(false), weapon2(false),
    jump(false), updated(false), pickup(false), facingDirection() {};//TODO pickup should be flase

  void serialize(sf::Packet & packet) {
    packet << player_id;
    packet << static_cast<sf::Int32>(movement);
    packet << attackMelee;
	  packet << attackRange;
    packet << weapon1;
    packet << weapon2;
    packet << jump;
    packet << pickup;
    facingDirection.serialize(packet);
  }

  void deserialize(sf::Packet & packet) {
    packet >> player_id;
    sf::Int32 movementInt = 0;
    packet >> movementInt;
    movement = static_cast<move_t>(movementInt);
    packet >> attackMelee;
	  packet >> attackRange;
    packet >> weapon1;
    packet >> weapon2;
    packet >> jump;
    packet >> pickup;
    facingDirection.deserialize(packet);
  }

  void print();

  std::string toString();
};
