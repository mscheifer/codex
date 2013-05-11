#include "Client.h"

namespace {
  gx::graphicEntity toGentity(const Entity& ent) {            
    auto pos = ent.getPosition();
    auto dir = ent.getDirection();
    gx::graphicEntity gentity;
    gentity.position =  gx::vector4f(static_cast<gx::vector4f::elem_t>(pos.x),
                                    static_cast<gx::vector4f::elem_t>(pos.y),
                                    static_cast<gx::vector4f::elem_t>(pos.z));
    gentity.direction = gx::vector3f(static_cast<gx::vector3f::elem_t>(dir.x),
                                     static_cast<gx::vector3f::elem_t>(dir.y),
                                     static_cast<gx::vector3f::elem_t>(dir.z));
    gentity.type = 0;
    return gentity;
  }
} //end nunnamed namespace

void NetworkClient::receiveMessages() {
  //receive from server and process
  sf::Packet packet;
  if (netRecv.receiveMessage(packet)) {
    ChatObject chatObj;
    uint32_t packetType;
    IdPacket newId(0);
    packet >> packetType;
    std::vector<gx::graphicEntity> entities;
    switch (packetType) {
      case CHAT:
        chatObj.deserialize(packet);
        this->chat.addChat(chatObj.getChat());
        break;
      case SGTR:
        s.deserialize(packet);
        for(auto playerP = s.players.begin(); playerP != s.players.end(); playerP++) {
          if(playerP->player_id != this->id) {
            auto gentity = toGentity(*playerP);
            gentity.type = 1;
            entities.push_back(gentity);
          }
        }
        for(auto entP = s.entities.begin(); entP != s.entities.end(); entP++) {
            auto gentity = toGentity(**entP);
            gentity.type = 3;
            entities.push_back(gentity);
        }
        auto pos = s.players[this->id].getPosition();
        gxClient.updatePosition(gx::vector4f(
          static_cast<gx::vector3f::elem_t>(pos.x),
          static_cast<gx::vector3f::elem_t>(pos.y),
          static_cast<gx::vector3f::elem_t>(pos.z)));
        gx::graphicEntity gentity;
        gentity.position  = gx::vector4f(0,0,0);
        gentity.direction = gx::vector3f(0,1,0);
        gentity.type = 4;
        entities.push_back(gentity); //add skybox
        gxClient.updateEntities(entities);
        //std::cout << "num entities received: " << entities.size() << std::endl;
        if (s.players[id].dead) { /*render death everytime ? */}
        //render WIN OR LOSE based on s.state
        //std::cout << pos.x << "," << pos.y << "," << pos.z << std::endl;
        sf::Listener::setPosition(pos.x, pos.y, pos.z);
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
    this->sendPacket = true; //TODO if nothing chnages dont send a packet
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
  AudioManager::playSound("m1",0,0,0);
  //AudioManager::playMusic("m1");

  std::cout << "Waiting for other players to join" << std::endl;
  while(true) {
	  sf::Packet initPacket;
    if (netRecv.receiveMessage(initPacket)) {
      std::cout << "received message" << std::endl;
      uint32_t packetType;
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
  while(true) {}
}
