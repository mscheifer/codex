#include "Client.h"
#include <iostream>
#include "AudioManager.h"
#include "Game.h"
int cycle = 0;
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
          
          if(cycle  == 100 ) {
             cycle = 0;
             std::cout << "Current health is " << (*playerP).getHealth() << std::endl;
          }

        }
        for(auto entP = s.walls.begin(); entP != s.walls.end(); entP++) {
          entities.push_back(*entP);
        }
        for(auto entP = s.projectiles.begin(); entP != s.projectiles.end(); entP++) {
          entities.push_back(*entP);
            AudioManager::processProjectileSound(**entP);
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
        gxClient.updateHUD(s.players[id]);
        //std::cout << "num entities received: " << entities.size() << std::endl;
        if (s.players[id].dead) { /*render death everytime ? */}
        //render WIN OR LOSE based on s.state
        sf::Listener::setPosition(pos.x, pos.y, pos.z);
        auto dir = s.players[this->id].getDirection();
        sf::Listener::setDirection(dir.x, dir.y, dir.z);

        //TODO not sure where to put this @bowen add to HUD here
        if( s.players[id].getPickupWeaponType() != UNK )
          std::cout << "can pick up weapon type " << WeaponNames[s.players[id].getPickupWeaponType()] << std::endl;
        
        //calculate proximity of players
        //TODO actually base this on proximity of players 
        //can do -- instead of ++ so we can have more intense if less players
        int proximity = 0;
        if(s.players[this->id].getPosition().x > 0){
          proximity++;
        }
        if(s.players[this->id].getPosition().y > 0){
          proximity++;
        }
        if(s.players[this->id].getPosition().y > 10){
          proximity++;
        }
        AudioManager::updateMusic(proximity);
        
        break;
    }
  }
}

void NetworkClient::processInput() {
  action = this->gxClient.handleInput();
  if(this->gxClient.closed()) { //add running  in ClientGameTimeAction ?
    this->running = false;
  }
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
  AudioManager::loadSounds();
  //AudioManager::playMusic("m1");

  //std::cout << "Waiting for other players to join" << std::endl;
  //TODO refactor the menu logic 
  bool joined = false;
  while(true) {
    cycle++;
    sf::Packet initPacket;
    this->gxClient.drawLobby();
    if (joined && this->gxClient.gameStart()) {
      initPacket << static_cast<sf::Uint32>(INIT); 
      netRecv.sendMessage(initPacket);
      joined =false; //only send packet once
    }
    initPacket.clear();
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
        joined = true;
      } else if (packetType == INIT) {
         //TODO: init the position
        break;
      }
    }
  }
  this->gxClient.disableCursor();
  std::cout << "game started" << std::endl;
  /*sf::Clock profilerTime;
  float processInputTime;
  float receiveMessagesTime;
  float drawTime;
  float sendPackTime;*/
  //  main run loop
  while(this->running) {
    //process input and send events
  
    //profilerTime.restart();
    this->processInput();
    //processInputTime = profilerTime.getElapsedTime().asMilliseconds();
    //profilerTime.restart();
    this->receiveMessages();
    //receiveMessagesTime = profilerTime.getElapsedTime().asMilliseconds();
    //profilerTime.restart();
    this->gxClient.draw();
    //drawTime = profilerTime.getElapsedTime().asMilliseconds();
    //profilerTime.restart();
    if(this->sendPacket) {//if dead player still should be able to chat?
      //this->action.print();
      this->netRecv.sendPacket<ClientGameTimeAction>(action);
      this->sendPacket = false;
    }
    //sendPackTime = profilerTime.getElapsedTime().asMilliseconds();
    //std::cout<<"processInput: "<< processInputTime <<"ms\treceiveMessagesTime: "<<
      //receiveMessagesTime <<"ms\tdrawTime: "<< drawTime <<"ms\tsendPackTime: "<< sendPackTime <<std::endl;
  }
}
