#pragma once
#include "Player.h"
#include "Entity.h"

struct ServerGameTimeRespond
{
	static const int packetType = SGTR;
  Player players[4];
	//Entity entities[15];	
  void serialize(sf::Packet & packet) const {
    for (int i=0;i<4;i++) 
      players[i].serialize(packet);
  }
  void deserialize(sf::Packet & packet) {
    for (int i=0;i<4;i++)
      players[i].deserialize(packet);
  }
};
