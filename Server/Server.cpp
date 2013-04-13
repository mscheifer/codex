#include "Server.h"

void NetworkServer::doServer(){
  sf::TcpSocket * client = new sf::TcpSocket;

  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;
  int tickspersecond = 30;
  ConfigManager::readConfig();

  while(true){
  double startTime = std::clock();

  // Wait for a connection
    if ( listener.accept(*client) == sf::Socket::Done ){
      std::cout << "New client connected: " << client->getRemoteAddress() << std::endl;
      client->setBlocking(false);
      clients.push_back(client);
      client = new sf::TcpSocket;
    }

    // Receive a message from the client
    //char buffer[1024];
    NetworkPacket buffer;

    std::size_t received = 0;

    for( unsigned int i = 0; i < clients.size(); i++){
      sf::TcpSocket::Status stat = clients[i]->receive(&buffer, NetworkPacket::maxPacketSize, received);

      if( stat == sf::Socket::Done ){
        //std::string message = clients[i]->getRemoteAddress().toString() + ": " + buffer.getType2Data();
        std::cout << clients[i]->getRemoteAddress().toString() << std::endl;

        //Send an answer to all clients
        for( unsigned int j = 0; j < clients.size(); j++){
            clients[j]->send(&buffer, sizeof(buffer));
        }
      }
    }

    double runTime = std::clock() - startTime;
    //std::printf("%lf", runTime);
    //std::cout << "start " << startTime << "run " << runTime << std::endl;
    sf::sleep( sf::seconds( (float)1.0/(float)tickspersecond - (float)runTime/CLOCKS_PER_SEC) );
  }
  delete client;
}