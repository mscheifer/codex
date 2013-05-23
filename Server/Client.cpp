#include "Client.h"
#include <iostream>
#include "AudioManager.h"
#include "Game.h"

namespace {
} //end nunnamed namespace

void NetworkClient::receiveMessages() {
  //receive from server and process
  sf::Packet packet;
  if (netRecv.receiveMessage(packet)) {
    ChatObject chatObj;
    sf::Uint32 packetType;
    packet >> packetType;
    std::vector<Entity*> entities;
    switch (packetType) {
      case CHAT:
        chatObj.deserialize(packet);
        this->chat.addChat(chatObj.getChat());
        break;
      case SGTR:
        this->s.deserialize(packet);
        for(auto playerP = s.players.begin(); playerP != s.players.end(); playerP++) {
          if(playerP->player_id != this->id) {
            //make sure the SGTR stays in scope
            entities.push_back(&(*playerP));
          }
        }
        for(auto entP = s.walls.begin(); entP != s.walls.end(); entP++) {
          entities.push_back(*entP);
        }
        for(auto entP = s.projectiles.begin(); entP != s.projectiles.end(); entP++) {
          entities.push_back(*entP);
            AudioManager::processEntitySound(**entP);
        }
        for(auto entP = s.powerups.begin(); entP != s.powerups.end(); entP++) {
          entities.push_back(*entP);
        }
        for(auto entP = s.weapons.begin(); entP != s.weapons.end(); entP++) {
          entities.push_back(*entP);
        }
        auto pos = s.players[this->id].getPosition();
        gxClient.updatePosition(gx::vector4f(pos.x,pos.y,pos.z));
        entities.push_back(&(this->skybox)); //add skybox
        gxClient.updateEntities(entities);
        //std::cout << "num entities received: " << entities.size() << std::endl;
        if (s.players[id].dead) { /*render death everytime ? */}
        //render WIN OR LOSE based on s.state
        sf::Listener::setPosition(pos.x, pos.y, pos.z);

        //TODO not sure where to put this
        if( s.players[id].getPickupWeaponType() != UNK )
          std::cout << "can pick up weapon type " << WeaponNames[s.players[id].getPickupWeaponType()] << std::endl;
        break;
    }
  }
}

void NetworkClient::processInput() {
  action = this->gxClient.handleInput();
  if(this->gxClient.closed()) { //add running  in ClientGameTimeAction ?
    this->running = false;
  }
  //ConfigManager::log(action.toString()); 
  if (action.updated) {
    action.player_id = id;
    this->sendPacket = true; 
    //ConfigManager::log(action.toString()); 
  }
}
/*
void NetworkClient::processInput(){
  bool updateAS = false;
  action.clear();
  
  if (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyReleased){        
        switch(event.key.code){
        case sf::Keyboard::Return:
          if(!chat.isTyping()){ //start typing
            chat.setBuffer("");
          }
          else{ //done typing
            sf::Packet packet;
            std:: string chatBuffer = chat.getBuffer();
            if(chatBuffer.size() > 0)
              netRecv.sendPacket<ChatObject>(ChatObject(chatBuffer));
            chat.setBuffer("");
          }
          chat.revertTyping();
          break;
        }
      }

      if (event.type == sf::Event::TextEntered){
        if( event.text.unicode >= 32 && event.text.unicode <= 125 && chat.isTyping() && chat.getBuffer().size() < 99)
          chat.appendBuffer((char) event.text.unicode);
        if (event.text.unicode == 8 &&chat.isTyping())
          chat.backspace();
      }
    }
  }

  if(updateAS && !s.players[id].dead){ //player should still be able to chat?
    sendPacket = true;
  }
}
*/
void NetworkClient::doClient() {
  ConfigManager::setupLog("client");
  sf::Listener::setDirection(1.f, 0.f, 0.f);
  AudioManager::loadSounds();
  //AudioManager::playMusic("m1");

  std::cout << "Waiting for other players to join" << std::endl;
  while(true) {
	  sf::Packet initPacket;
    if (netRecv.receiveMessage(initPacket)) {
      std::cout << "received message" << std::endl;
      sf::Uint32 packetType;
      initPacket >> packetType;
      if (packetType == JOINID) {
        IdPacket newId(0);
        newId.deserialize(initPacket);
        this->id = newId.id;
        std::cout << "USERID: " << this->id << std::endl;
        this->action.player_id = id;
      } else if (packetType == INIT) {
         //TODO: init the position
        break;
      }
	  }
  }
  std::cout << "game started" << std::endl;
  //  main run loop
  //for(int i = 0; i < 4; i++) {
  while(this->running) {
    //process input and send events
    this->processInput();
    this->receiveMessages();
    this->gxClient.draw();
    if(this->sendPacket) {//if dead player still should be able to chat?
      //this->action.print();
      this->netRecv.sendPacket<ClientGameTimeAction>(action);
      this->sendPacket = false;
    }
  }
}
