#pragma once
#include "Player.h"
#include "Entity.h"
#include "PowerUp.h"
#include "EntityPool.h"

struct ServerGameTimeRespond
{
  static const int packetType = SGTR;
  std::vector<Player> players;
  std::vector<Wall*> walls;
  std::vector<Projectile*> projectiles;
  std::vector<PowerUp*> powerups;
  std::vector<Weapon*> weapons;
  EntityPool * objPool;
  Game_State state;
  ServerGameTimeRespond():state(PLAYING) {}
  //Client's constructor
  ServerGameTimeRespond(EntityPool* pool): objPool(pool), state(PLAYING) {}

  void serialize(sf::Packet & packet) const {
    sf::Uint32 size = static_cast<sf::Uint32>(players.size());
    packet << size;
    for (unsigned int i = 0; i < players.size();i++) { 
      players[i].serialize(packet);
    }

    size = static_cast<sf::Uint32>(walls.size());
    packet << size;
    for(unsigned int i = 0; i < size; i++) {
      walls[i]->serialize(packet);
    }
    size = static_cast<sf::Uint32>(projectiles.size());
    packet << size;
    for(unsigned int i = 0; i < size; i++) {
      projectiles[i]->serialize(packet);
    }
    size = static_cast<sf::Uint32>(powerups.size());
    packet << size;
    for(unsigned int i = 0; i < size; i++) {
      powerups[i]->serialize(packet);
    }
    size = static_cast<sf::Uint32>(weapons.size());
    packet << size;
    for(unsigned int i = 0; i < size; i++) {
      weapons[i]->serialize(packet);
    }
    size = static_cast<sf::Uint32>(state);
    packet<<size;
  }

  //make sure to clear the packet's sizes
  void deserialize(sf::Packet & packet) {
    objPool->reset();
    players.clear();
    
    sf::Uint32 size;
    packet >> size;
    for (unsigned int i=0; i < size; i++) {
      Player p;
      p.deserialize(packet);
      this->players.push_back(p);
    }
    this->walls.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      Wall* newWall = objPool->createWall(); 
      newWall->deserialize(packet);
      this->walls.push_back(newWall);
    }
    this->projectiles.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      Projectile* newProjectile = objPool->createProjectile(); 
      newProjectile->deserialize(packet);
      this->projectiles.push_back(newProjectile);
    }
    this->powerups.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      PowerUp* newPowerUp = objPool->createPowerUp(); 
      newPowerUp->deserialize(packet);
      this->powerups.push_back(newPowerUp);
    }
    this->weapons.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      Weapon* newWeapon = objPool->createWeapon(); 
      newWeapon->deserialize(packet);
      this->weapons.push_back(newWeapon);
    }
    sf::Uint32 temp;
    packet >> temp;
    state = static_cast<Game_State>(temp);
  }

  std::string toString(){
    std::stringstream ss;
    ss << "player size " << players.size() << std::endl;
    for( unsigned int i = 0; i < players.size() ; i++){
      ss << players[i].toString();
    }
    ss << "walls size " << walls.size() << std::endl;
    for( unsigned int i = 0; i < walls.size() ; i++){
      ss << walls[i]->toString();
    }
    ss << "proj size " << projectiles.size() << std::endl;
    for( unsigned int i = 0; i < projectiles.size() ; i++){
      ss << projectiles[i]->toString2();
    }
    ss << "powerUps size " << powerups.size() << std::endl;
    for( unsigned int i = 0; i < powerups.size() ; i++){
      ss << powerups[i]->toString();
    }
    ss << "weapons size " << weapons.size() << std::endl;
    for( unsigned int i = 0 ; i < weapons.size() ; i++){
      ss << weapons[i]->toString();
    }
    return ss.str();
  }

  std::string getAllSizes(){
    std::stringstream ss;
    ss << " player size " << players.size();
    ss << " walls size " << walls.size();
    ss << " proj size " << projectiles.size();
    ss << " powerUps size " << powerups.size();
    ss << " weapons size " << weapons.size();
    return ss.str();
  }
};
