#ifndef SERVER
#define SERVER
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
//#include <time.h>
#include <SFML/System/Clock.hpp>
#include "ConfigManager.h"
#include "NetworkServices.h"
#include "ClientGameTimeAction.h"
#include "Game.h"
#include "ServerGameTimeRespond.h"

class NetworkServer{
public:
  ServerServices server;
  Game game;

  NetworkServer(): server() {}

  ~NetworkServer() {}
  
  void receiveMessages(int i);  
  void doServer();
};
#endif
