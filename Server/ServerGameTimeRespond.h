#pragma once
#include "Player.h"
#include "Entity.h"
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
  ServerGameTimeRespond(EntityPool* pool): state(PLAYING), objPool(pool){}

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
  }

  //make sure to clear the packet's sizes
  void deserialize(sf::Packet & packet) {
    objPool->reset();
    players.clear();
    sf::Uint32 size = 0;
    packet >> size;
    for (unsigned int i=0; i < size; i++) {
      Player p = Player();
      p.deserialize(packet);
      players.push_back(p);
    }
    walls.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      Wall* newWall = (Wall*) objPool->createEntity(static_cast<sf::Uint32>(WALL)); 
      newWall->deserialize(packet);
      walls.push_back(newWall);
    }
    projectiles.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      Projectile* newProjectile = (Projectile*) objPool->createEntity(static_cast<sf::Uint32>(PROJECTILE)); 
      newProjectile->deserialize(packet);
      projectiles.push_back(newProjectile);
    }
    powerups.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      PowerUp* newPowerUp = (PowerUp*) objPool->createEntity(static_cast<sf::Uint32>(POWER_UP)); 
      newPowerUp->deserialize(packet);
      powerups.push_back(newPowerUp);
    }
    weapons.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      Weapon* newWeapon = (Weapon*) objPool->createEntity(static_cast<sf::Uint32>(WEAPON)); 
      newWeapon->deserialize(packet);
      weapons.push_back(newWeapon);
    }
  }
};
