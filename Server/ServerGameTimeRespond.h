#pragma once
#include "Player.h"
#include "Entity.h"

struct ServerGameTimeRespond
{
  static const int packetType = SGTR;
  //Player players[4];
  std::vector<Player> players;
  std::vector<Entity> entities;
  Game_State state;
  ServerGameTimeRespond():state(PLAYING) {}

  void serialize(sf::Packet & packet) const {
    sf::Uint32 size = static_cast<sf::Uint32>(players.size());
    packet << size;
    for (int i = 0; i < players.size();i++) { 
      players[i].serialize(packet);
    }

    size = static_cast<sf::Uint32>(entities.size());
    packet << size;
    for(unsigned int i = 0; i < size; i++) {
      entities[i].serialize(packet);
    }
  }

  //make sure to clear the packet's sizes
  void deserialize(sf::Packet & packet) {
	  entities.clear();
    players.clear();
    
    sf::Uint32 size = 0;
    packet >> size;
    for (int i=0; i<size; i++) {
      Player p = Player();
      p.deserialize(packet);
      players.push_back(p);
    }

    size = 0;
    packet >> size;
    for(unsigned int i = 0; i < size; i++) {
      Entity newEntity = Entity(); //TODO should we do it lke this?
      newEntity.deserialize(packet);
      entities.push_back(newEntity);
    }
  }
};
