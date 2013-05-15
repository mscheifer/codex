#pragma once
#include "Player.h"
#include "Entity.h"
#include "EntityPool.h"

struct ServerGameTimeRespond
{
  static const int packetType = SGTR;
  std::vector<Player> players;
  std::vector<Entity*> entities;
  Game_State state;
  EntityPool* objPool;
  ServerGameTimeRespond():state(PLAYING) {}
  //Client's constructor
  ServerGameTimeRespond(EntityPool* pool): state(PLAYING), objPool(pool) {}

  void serialize(sf::Packet & packet) const {
    sf::Uint32 size = static_cast<sf::Uint32>(players.size());
    packet << size;
    for (unsigned int i = 0; i < players.size();i++) { 
      players[i].serialize(packet);
    }

    size = static_cast<sf::Uint32>(entities.size());
    packet << size;
    for(unsigned int i = 0; i < size; i++) {
      entities[i]->serialize(packet);
    }
  }

  //make sure to clear the packet's sizes
  void deserialize(sf::Packet & packet) {
    objPool->reset();
    players.clear();
    
    sf::Uint32 size;
    packet >> size;
    for (unsigned int i=0; i < size; i++) {
      Player p = Player();
      p.deserialize(packet);
      this->players.push_back(p);
    }
    this->entities.clear();
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      sf::Uint32 packet_type;
      packet >> packet_type;
      Entity* newEntity = objPool->createEntity(packet_type); 
      newEntity->deserialize(packet);
      this->entities.push_back(newEntity);
    }
  }
};
