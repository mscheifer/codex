#include "Server.h"

void NetworkServer::doServer(){
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;
  int tickspersecond = 30;
  sf::Clock clock;
  ServerServices server;
  
  while (server.size()<NUM_PLAYERS) {
    server.getNewClient();
  }
  sf::Packet initPacket;
  size_t initGame = 0x80000000;    
  initPacket<<initGame;
  server.sendToAll(initPacket);
  std::cout<<"server start game"<<std::endl;

  while(true){
    clock.restart();
    for( unsigned int i = 0; i < server.size(); i++){
      /* maybe put this in a method just like in client*/
      sf::Packet packet;
      if(server.receiveMessage(packet,i)){
        sf::Packet copy =packet;
        switch (processMeta(packet)) {

         ClientGameTimeAction cgta;
         case CGTA:
           cgta.deserialize(packet);
           cgta.print();
           break;
        case CHAT:
          server.sendToAll(copy); //right now just echoing what received
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

