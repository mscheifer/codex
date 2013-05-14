#include "ClientGameTimeAction.h"
#include <iostream>

bool ClientGameTimeAction::operator==(const ClientGameTimeAction & other) const{
  if (this->player_id != other.player_id) return false; 
  if (this->movement != other.movement) return false; 
  if (this->attackMelee != other.attackMelee) return false; 
  if (this->weapon1 != other.weapon1) return false; 
  if (this->weapon2 != other.weapon2) return false; 
  if (this->jump != other.jump) return false; 
  if (this->facingDirection != other.facingDirection) return false; 
  return true;
}

bool ClientGameTimeAction::operator!=(const ClientGameTimeAction & other) const{
  return !(*this==other);
}

void ClientGameTimeAction::clear() {
  movement = NULL_DIR;
  attackMelee = attackRange = weapon1 = weapon2 = jump = false;
  facingDirection.x = facingDirection.z = 0;
  facingDirection.y = 0;
  updated = false;
}

void ClientGameTimeAction::print() {
  std::cout << "id " << player_id << std::endl
    << "mov " << movement << std::endl
    << "attackMelee " << attackMelee << std::endl
    << "range " << attackRange << std::endl
    << "weapon1 " << weapon1 << std::endl
    << "weapon2 " << weapon2 << std::endl
    << "jump " << jump << std::endl
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
     << "facingDirection " << facingDirection.x << " "<< facingDirection.y
     << " " << facingDirection.z << std::endl << std::endl;
  return ss.str();
}
