#ifndef SERVER_H
#define SERVER_H
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/System/Clock.hpp>
#include "ConfigManager.h"
#include "NetworkServices.h"
#include "ClientGameTimeAction.h"
#include "Game.h"
#include "ServerGameTimeRespond.h"

class NetworkServer{
  int connectionCount;
  StartGamePacket startTheGame;
  Game_State gameState;
public:
  ServerServices server;
  Game game;
  ClientGameTimeAction pPacket;

  NetworkServer(): connectionCount(0), startTheGame(), gameState(WAIT), server() {}

  ~NetworkServer() {}
  
  void combinePackets(ClientGameTimeAction & a);
  void receiveMessages(int i);  
  void doServer();
};
#endif
