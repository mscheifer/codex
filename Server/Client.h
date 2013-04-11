#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

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

  void doClient();

  ~NetworkClient(){
  }


};