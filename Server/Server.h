#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "NetworkPacket.h"
#include <time.h>
#include "ConfigManager.h"
#include "NetworkServices.h"

class NetworkServer{

public:
  NetworkServer(){

  }

  ~NetworkServer(){

  }
  
  //run the server
  void doServer();
};