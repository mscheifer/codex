#include "Server.h"

void NetworkServer::doServer(){
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;
  int tickspersecond = 30;
  sf::Clock clock;
  ServerServices server;

  while(true){
   clock.restart();
   server.getNewClient();
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
      }
    }

    sf::sleep( sf::milliseconds( (int)((float)1.0/(float)tickspersecond*1000 - (float)clock.getElapsedTime().asMilliseconds())) );
    clock.restart();
  }
}

