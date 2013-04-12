#pragma once
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include "NetworkPacket.h"

class NetworkClient{

private:
  sf::TcpSocket socket;
  sf::Socket::Status s;
  bool validIpAddress;
  bool pressedLeft;
  bool pressedRight;

public:
  NetworkClient(){
    validIpAddress = true;
    pressedLeft = false;
    pressedRight = false;
  }

  ~NetworkClient(){
  }

  void doClient();
};