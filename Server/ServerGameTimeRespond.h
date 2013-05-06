#pragma once
#include "Player.h"
#include "Entity.h"

struct ServerGameTimeRespond
{
  static const int packetType = SGTR;
  Player players[4];
  std::vector<Entity *> entities;
  Game_State state;
  ServerGameTimeRespond():state(PLAYING) {}

  void serialize(sf::Packet & packet) const {
    for (int i = 0; i < 4;i++) { 
      players[i].serialize(packet);
    }
    sf::Uint32 size = static_cast<sf::Uint32>(entities.size());
    packet << size;
    for(unsigned int i = 0; i < size; i++) {
      entities[i]->serialize(packet);
    }
  }
  void deserialize(sf::Packet & packet) {
	  // TODO: Matt&Bowen is this correct??
    for (int i=0;i<4;i++) {
      players[i].deserialize(packet);
    }

    sf::Uint32 size = 0;
    packet >> size;
    entities.clear();
    for(unsigned int i = 0; i < size; i++) {
      Entity* newEntity = new Entity();
      newEntity->deserialize(packet);
      entities.push_back( newEntity );
    }
  }
};
