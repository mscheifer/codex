#include "Server.h"

void NetworkServer::doServer(){
  sf::TcpSocket * client = new sf::TcpSocket;

  while(true){
  // Wait for a connection
    if ( listener.accept(*client) == sf::Socket::Done ){
      std::cout << "New client connected: " << client->getRemoteAddress() << std::endl;
      client->setBlocking(false);
      clients.push_back(client);
      client = new sf::TcpSocket;
    }

    // Receive a message from the client
    char buffer[1024];
    std::size_t received = 0;

    for( unsigned int i = 0; i < clients.size(); i++){
      sf::TcpSocket::Status stat = clients[i]->receive(buffer, sizeof(buffer), received);
      if( stat == sf::Socket::Done ){
        std::string message = clients[i]->getRemoteAddress().toString() + ": " + buffer;
        std::cout << message << std::endl;

        //Send an answer to all clients
        for( unsigned int j = 0; j < clients.size(); j++){
            clients[j]->send(message.c_str(), message.size() + 1);
        }
      }
    }

  }
  delete client;
}