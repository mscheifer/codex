#include "Server.h"

void NetworkServer::doServer(){
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;
  int tickspersecond = 30;
  sf::Clock clock;
  ServerServices server;

  while(true){
   clock.restart();
   //double startTime = std::clock();
      server.getNewClient();

    // Receive a message from the client
    //char buffer[1024];
    //NetworkPacket buffer;

     for( unsigned int i = 0; i < server.clients.size(); i++){
      if(server.clients[i]->receiveMessage()){
        clock_t recv = std::atoi(((NetworkPacket*) server.clients[i]->networkBuffer)->chat.msg) ; 
        std::cout << recv << std::endl;
        server.clients[i]->sendMessage( *(NetworkPacket*)server.clients[i]->networkBuffer );
      }

      /*sf::TcpSocket::Status stat = clients[i]->receive(&buffer, NetworkPacket::maxPacketSize, received);

      if( stat == sf::Socket::Done ){
        //std::string message = clients[i]->getRemoteAddress().toString() + ": " + buffer.getType2Data();
        std::cout << clients[i]->getRemoteAddress().toString() << std::endl;

        //Send an answer to all clients
        for( unsigned int j = 0; j < clients.size(); j++){
            clients[j]->send(&buffer, sizeof(buffer));
        }
      }*/
    }

    //double runTime = std::clock() - startTime;
    //std::printf("%lf", runTime);
    //unsigned int time = clock.getElapsedTime();
    //std::cout << clock.getElapsedTime().asMilliseconds() << " ms" << std::endl;
    //std::cout << "start " << startTime << "run " << runTime << std::endl;
    sf::sleep( sf::milliseconds( (int)((float)1.0/(float)tickspersecond*1000 - (float)clock.getElapsedTime().asMilliseconds())) );
    clock.restart();
  }
}

