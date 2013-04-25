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

class NetworkServer{

public:
  NetworkServer(){

  }

  ~NetworkServer(){

  }
  
  void doServer();
};
#endif