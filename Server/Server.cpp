#include "Server.h"

void NetworkServer::receiveMessages(int i) {
    sf::Packet packet;
    if(this->server.receiveMessage(packet,i)) {
      sf::Packet copy = packet; //TODO: maybe we don't need this. fix later
      ClientGameTimeAction cgta;
      uint32_t packetType;
      packet >> packetType;
      
      switch (packetType) {
        case CGTA:
          cgta.deserialize(packet);
          cgta.print();
          if(!this->server.sendPacketToAll<ServerGameTimeRespond>
                                                  (game.evaluate(cgta))) {
            std::cout << "Error sending cgta to everybody" << std::endl;
          }
          break;
        case CHAT:
          this->server.sendToAll(copy); //right now just echoing what received
          break;
        default:
          std::cout << "Error: received bad packet: " << packetType<< std::endl;
          break;
      }
    }
}

void NetworkServer::doServer() {
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;
  const int tickspersecond = 30;
  const int tick_length = 1000 / tickspersecond;
  sf::Clock clock;
  
  while (server.size() < NUM_PLAYERS) {
    if(server.getNewClient())
    {
      IdPacket newPacket = IdPacket(game.join());
      if(!server.sendPacket<IdPacket>(newPacket,server.size() - 1)) {
        std::cout << "Error sending game join packet" << std::endl;
	  }
	  else
	    std::cout << "I sent the id " << std::endl;
    }
  }
  //choose minotaur
  game.chooseMinotaur();

  sf::Packet initPacket;
  initPacket << INIT;
  if(!server.sendToAll(initPacket)) {
	  std::cout << "Error sending game start packet" << std::endl;
  }
  std::cout << "server start game" << std::endl;

  while(true) {
    clock.restart();
    for(unsigned int i = 0; i < server.size(); i++){
      this->receiveMessages(i);
      /* maybe put this in a method just like in client*/
    }
    sf::sleep( sf::milliseconds( tick_length - clock.getElapsedTime().asMilliseconds()) );
  }
} 

