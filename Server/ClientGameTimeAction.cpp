#include "ClientGameTimeAction.h"
#include <iostream>

bool ClientGameTimeAction::operator==(const ClientGameTimeAction & other) const{
  if (this->player_id != other.player_id) return false; 
  if (this->movement != other.movement) return false; 
  if (this->attackMelee != other.attackMelee) return false; 
  if (this->weapon1 != other.weapon1) return false; 
  if (this->weapon2 != other.weapon2) return false; 
  if (this->jump != other.jump) return false; 
  if (this->pickup != other.pickup) return false;
  if (this->facingDirection != other.facingDirection) return false; 
  return true;
}

bool ClientGameTimeAction::operator!=(const ClientGameTimeAction & other) const{
  return !(*this==other);
}

void ClientGameTimeAction::clear() {
  movement = NULL_DIR;
  attackMelee = attackRange = weapon1 = weapon2 = jump = pickup = false;
  facingDirection.x = facingDirection.z = 0;
  facingDirection.y = 0;
  updated = false;
}
  void ClientGameTimeAction::serialize(sf::Packet & packet) const {
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

  void ClientGameTimeAction::deserialize(sf::Packet & packet) {
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


void ClientGameTimeAction::print() {
  std::cout << "id " << player_id << std::endl
    << "mov " << movement << std::endl
    << "attackMelee " << attackMelee << std::endl
    << "range " << attackRange << std::endl
    << "weapon1 " << weapon1 << std::endl
    << "weapon2 " << weapon2 << std::endl
    << "jump " << jump << std::endl
    << "pickup " << pickup << std::endl
    << "facingDirection " << facingDirection.x << " "<< facingDirection.y << " "
    << facingDirection.z << std::endl; 
}

std::string ClientGameTimeAction::toString() {
  std::stringstream ss;
  ss << "id " << player_id << std::endl
     << "mov " << movement << std::endl
     << "attackMelee " << attackMelee << std::endl
     << "range " << attackRange << std::endl
     << "weapon1 " << weapon1 << std::endl
     << "weapon2 " << weapon2 << std::endl
     << "jump " << jump << std::endl
     << "pickup " << pickup << std::endl
     << "facingDirection " << facingDirection.x << " "<< facingDirection.y
     << " " << facingDirection.z << std::endl << std::endl;
  return ss.str();
}
