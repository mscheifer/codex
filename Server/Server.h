#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>


class NetworkServer{

private:
    std::vector<sf::TcpSocket*> clients;
    sf::TcpListener listener;

public:
  NetworkServer(){
    sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
    std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;

    listener.setBlocking(false);
    listener.listen(55001); //49152–65535 ports ok to use
  };
  
  //run the server
  void doServer();

  ~NetworkServer(){
    for( unsigned int i = 0; i < clients.size(); i++ ){
      delete clients[i];
    }
  }
};