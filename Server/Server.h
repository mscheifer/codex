#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "NetworkPacket.h"

class NetworkServer{

private:
    std::vector<sf::TcpSocket*> clients;
    sf::TcpListener listener;

public:
  NetworkServer(){
    listener.setBlocking(false);
    listener.listen(55001); //49152–65535 ports ok to use
  };

  ~NetworkServer(){
    for( unsigned int i = 0; i < clients.size(); i++ ){
      delete clients[i];
    }
  }
  
  //run the server
  void doServer();
};