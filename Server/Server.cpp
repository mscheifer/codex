#include "Server.h"

void NetworkServer::combinePackets(ClientGameTimeAction & a) {
  pPacket.player_id = a.player_id;
  pPacket.movement = a.movement;
  pPacket.attackMelee |= a.attackMelee;
	pPacket.attackRange |= a.attackRange;
	pPacket.weapon1 |= a.weapon1;
	pPacket.weapon2 |= a.weapon2;
	pPacket.jump |= a.jump;
	pPacket.facingDirection = a.facingDirection;
}
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
          combinePackets(cgta);
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
  ConfigManager::log(pPacket.toString()); 
  game.evaluate(pPacket);
  pPacket.clear();
}

void NetworkServer::doServer() {
  ConfigManager::setupLog("server");
  ConfigManager::log("lol");
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;
  const int ticksPerSecond = 30;
  const int tickLength = 1000 / ticksPerSecond;
  sf::Clock clock;
  
  //send id to to the player
  while (server.size() < ConfigManager::numPlayers()) {
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

  //send init packet to the players
  for(unsigned int i = 0; i < server.size(); i++){
    //TODO this relies on server i to match the player_id
    if(!server.sendPacket<InitPacket>(game.getInitPacket(i),i)) {
        std::cout << "Error sending init packet" << std::endl;
	    }
  }

  //if(!server.sendToAll(initPacket)) {
	//  std::cout << "Error sending game start packet" << std::endl;
  //}
  std::cout << "server start game" << std::endl;
  int i = 0;
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
      std::cout << "Error sending sgtr to everybody" << std::endl;
    }

    //4. go back to sleep slave.
    sf::sleep( sf::milliseconds( tickLength -
                                 clock.getElapsedTime().asMilliseconds()) );
  }
} 

