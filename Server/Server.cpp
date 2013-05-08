#include "Server.h"

void NetworkServer::receiveMessages(int i) {
  sf::Packet packet;
    while(this->server.receiveMessage(packet,i)) {
      sf::Packet copy = packet; //TODO: maybe we don't need this. fix later
      ClientGameTimeAction cgta;
      uint32_t packetType;
      packet >> packetType;
      switch (packetType) {
        case CGTA:
          cgta.deserialize(packet);
          if (cgta == pPacket) 
            continue; 
          else 
            pPacket = cgta;
          game.evaluate(cgta);
          //cgta.print();
        break;
        case CHAT: //chat should be part of CGTA  
        this->server.sendToAll(copy); //right now just echoing what received
        break;
      default:
        std::cout << "Error: received bad packet: " << packetType<< std::endl;
        break;
    }
  }
  pPacket.clear();
}

void NetworkServer::doServer() {
  ConfigManager::setupLog("server");
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
	    } else {
	      std::cout << "I sent the id " << std::endl;
      }
    }
  }
  //choose minotaur
  game.chooseMinotaur();

  for(unsigned int i = 0; i < server.size(); i++){
    //TODO this relies on server i to match the player_id
    if(!server.sendPacket<InitPacket>(game.getInitPacket(i),i)) {
        std::cout << "Error sending game join packet" << std::endl;
	    }
  }

  //if(!server.sendToAll(initPacket)) {
	//  std::cout << "Error sending game start packet" << std::endl;
  //}
  std::cout << "server start game" << std::endl;

  while(true) {
    clock.restart();
    
    // don't fuck with this order here

    //1. handle incoming packet
    for(unsigned int i = 0; i < server.size(); i++){
      this->receiveMessages(i);
      /* maybe put this in a method just like in client*/
    }

    //2. update all entities and resolve collision

    game.updateAndResolveCollision();


    //3. prep and send response to everyone
	  if(!this->server.sendPacketToAll<ServerGameTimeRespond>( game.prepResponse() ) ) {
          std::cout << "Error sending cgta to everybody" << std::endl;
    }

    //4. go back to sleep slave.
    sf::sleep( sf::milliseconds( tick_length -
                                 clock.getElapsedTime().asMilliseconds()) );
  }
} 

