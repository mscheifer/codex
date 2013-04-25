//#pragma once
#ifndef SERVER
#define SERVER
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <SFML/System/Clock.hpp>
#include "ConfigManager.h"
#include "NetworkServices.h"
#include "ClientGameTimeAction.h"

class NetworkServer{

public:
  ServerServices server;
  NetworkServer(){

  }

  ~NetworkServer(){

  }
  void receiveMessages(int i);  
  void doServer();
};
#endif
