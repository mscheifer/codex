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

     for( unsigned int i = 0; i < server.size(); i++){
       sf::Packet packet;
       if(server.receiveMessage(packet,i)){
         int32_t packetType;
         packet >> packetType;
         switch (packetType) {
         case CHAT:
           server.sendToAll(packet); //right now just echoing what received
           break;
         default: 
           break;
         }
         //int32_t packetType;
         //packet >> packetType; 
         //std::string recvTime;
         //packet >>recvTime;
         //clock_t recv = std::atoi(recvTime.c_str());
         //std::cout << recv << std::endl;
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

