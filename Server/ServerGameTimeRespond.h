#pragma once
#include "Player.h"
#include "Entity.h"

struct ServerGameTimeRespond
{
	static const int packetType = SGTR;
  Player players[4];
  Game_State state;
  ServerGameTimeRespond():state(PLAYING) {}

	//Entity entities[15];	
  void serialize(sf::Packet & packet) {
    for (int i=0;i<4;i++) 
      players[i].serialize(packet);
  }
  void deserialize(sf::Packet & packet) {
    for (int i=0;i<4;i++)
      players[i].deserialize(packet);
  }
};