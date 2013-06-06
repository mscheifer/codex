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
    ClientGameTimeAction cgta;
    bool status;
    std::string name;
    sf::Uint32 packetType;
    packet >> packetType;
    switch (packetType) {
      case CGTA:
        packetReceived = true;
        cgta.deserialize(packet);
        combinePackets(cgta);
        //ConfigManager::log(cgta.toString()); 
        break;
      case INIT:
        packet >> status;
        connectionCount = (status) ? (connectionCount+1):(connectionCount-1);
        startTheGame.changeStatus(i);
        if (status) {
          packet >> name;
          game.assignName(name,i);
        }
        this->server.sendPacketToAll<StartGamePacket>(startTheGame);
        break;
      default:
        std::cout << "Error server: received bad packet: " << packetType << std::endl;
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
  ServerGameTimeRespond sgtr;

  //scores should not change
  game.initScores();
 
  //lobby code
  while (connectionCount < ConfigManager::numPlayers()) {
    // add a new client if can
    if(server.size() < ConfigManager::numPlayers() && server.getNewClient()){
      IdPacket newPacket = IdPacket(game.join());
      //send id to to the player
      if(!server.sendPacket<IdPacket>(newPacket,server.size() - 1)) {
        std::cout << "Error sending game join packet" << std::endl;
	    } else {
	      std::cout << "I sent the id " << std::endl;
        //after sending ID, broad cast to all players
        startTheGame.addPlayer();
        this->server.sendPacketToAll<StartGamePacket>(startTheGame);
      }
    }
    // wait for players to start
    for(unsigned int i = 0; i < server.size(); i++){
      sf::Packet packet;
      this->receiveMessages(i);
    }
  }
  //game loop
  while(true)
  {
    //Wait for players to start
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
    std::cout << "server start game in 5 seconds" << std::endl;
    clock.restart();
    while(true) {
      if (gameState == PLAYING) {
        clock.restart();
        game.clearEvents();
        // don't fuck with this order here
 
        //1. handle incoming packet
        for(unsigned int i = 0; i < server.size(); i++){
          this->receiveMessages(i);
        }

        //2. update all entities and resolve collision
        game.updateAndResolveCollision();

        game.prepResponse(sgtr);
        gameState = sgtr.state;
        //3. prep and send response to everyone
	      if(!this->server.sendPacketToAll<ServerGameTimeRespond>( sgtr ) ) {
          std::cout << "Error sending sgtr to everybody" << std::endl;
          ConfigManager::log(std::string("Error sending sgtr to everyone"));
        }

        // Go back to the lobby to wait for the game to restart
        if(gameState != Game_State::PLAYING ) {
          clock.restart();
          break;
        }
        //4. go back to sleep slave.
        sf::Int32 sleepAmount = static_cast<sf::Int32>(ConfigManager::serverTickLengthMilli()) - clock.getElapsedTime().asMilliseconds();
        if(sleepAmount < 0 )
          std::cout << "Error, sleep negative amount. server can't run at this tick speed, lower it" << std::endl;
        sf::sleep( sf::milliseconds(sleepAmount) );
      } else {
        if (clock.getElapsedTime().asSeconds() > StringToNumber<unsigned int>(ConfigManager::configMap["countdown"])) { //TODO make this 5 sec again
          gameState = PLAYING; 
        }
      }
    }
  }
} 

