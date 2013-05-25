#include "Server.h"

void NetworkServer::combinePackets(ClientGameTimeAction & a) {
  pPacket.player_id = a.player_id;
  pPacket.movement = a.movement;
  pPacket.attackMelee |= a.attackMelee;
	pPacket.attackRange |= a.attackRange;
	pPacket.weapon1 |= a.weapon1;
	pPacket.weapon2 |= a.weapon2;
	pPacket.jump |= a.jump;
  pPacket.pickup |= a.pickup;
  pPacket.switchWeapon |= a.switchWeapon;
  pPacket.facingDirection = a.facingDirection;
}

void NetworkServer::receiveMessages(int i) {
  sf::Packet packet;
  bool packetReceived = false;
  while(this->server.receiveMessage(packet,i)) {
    packetReceived = true;
    sf::Packet copy = packet; //TODO: maybe we don't need this. fix later
    ClientGameTimeAction cgta;
    sf::Uint32 packetType;
    packet >> packetType;
    switch (packetType) {
      case CGTA:
        cgta.deserialize(packet);
        combinePackets(cgta);
        //ConfigManager::log(cgta.toString()); 
        break;
      case CHAT: //chat should be part of CGTA  
        this->server.sendToAll(copy); //right now just echoing what received
        break;
      default:
        std::cout << "Error: received bad packet: " << packetType<< std::endl;
        break;
    }
  }
  if (packetReceived) {
    //ConfigManager::log("------------------------------------");
    //ConfigManager::log(pPacket.toString()); 
    game.evaluate(pPacket);

    pPacket.clear();
  }
}

void NetworkServer::doServer() {
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << std::endl;
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

  StartGamePacket startTheGame;
  while(true)
  {
    this->server.sendPacketToAll<StartGamePacket>(startTheGame);
    //Wait for players to start
    unsigned int connectionCount = 0;
    while (connectionCount < ConfigManager::numPlayers()) {
      for(unsigned int i = 0; i < server.size(); i++){
        sf::Packet packet;
        if (this->server.receiveMessage(packet,i)) {
          sf::Uint32 packetType;
          packet >> packetType;
          if (packetType == INIT) {
              connectionCount++;
          }
        }
      }
    }

    game.restartGame();
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
    while(true) {
      clock.restart();
    
      game.clearEvents();
      // don't fuck with this order here

      //1. handle incoming packet
      for(unsigned int i = 0; i < server.size(); i++){
        this->receiveMessages(i);
        /* maybe put this in a method just like in client*/
      }

      //2. update all entities and resolve collision
      game.updateAndResolveCollision();

      ServerGameTimeRespond gtr = game.prepResponse();
      //3. prep and send response to everyone
	    if(!this->server.sendPacketToAll<ServerGameTimeRespond>( gtr ) ) {
        std::cout << "Error sending sgtr to everybody" << std::endl;
      }

      // Go back to the lobby to wait for the game to restart
      if(gtr.state != Game_State::PLAYING && ConfigManager::gameRestart())
        break;

      //4. go back to sleep slave.
      sf::sleep( sf::milliseconds( static_cast<sf::Int32>(ConfigManager::serverTickLengthMilli()) -
                                   clock.getElapsedTime().asMilliseconds()) );
    }
  }
} 

