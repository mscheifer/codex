#include "Client.h"

void NetworkClient::receiveMessages() {
      //receive from server and process
      sf::Packet packet;
      if (netRecv.receiveMessage(packet)) {
        ChatObject chatObj;
        uint32_t packetType;
        IdPacket newId(0);
        packet >> packetType;
        std::vector<std::pair<gx::vector3,int>> entities;
        switch (packetType) {
        case CHAT:
          chatObj.deserialize(packet);
          this->chat.addChat(chatObj.getChat());
          break;
        case SGTR:
          s.deserialize(packet);
          for(size_t i = 0; i < 4; i++) {
            auto pos = s.players[i].getPosition();
            entities.push_back(std::make_pair(gx::vector3(pos.x,pos.y,pos.z),0));
			std::cout << "recieved player at: " << gx::vector3(pos.x,pos.y,pos.z) << std::endl;
          }
          gxClient.updateEntities(entities);
          if (s.players[id].dead) { /*render death everytime ? */} ;
          //render WIN OR LOSE based on s.state
          break;
        case JOINID:
          newId.deserialize(packet);
          this->id = newId.id;
          std::cout << "USERID: " << this->id << std::endl;
          this->action.player_id = id;
          break;
        }
      }
}

void NetworkClient::processInput(gx::userInput ui) {
  if(ui.stopped) {
    this->running = false;
  }
  action.clear();
  if(ui.jumped) {
    action.jump = true;
  }
  switch (ui.move) {
    case gx::FORWARD:
      action.movement = FORWARD;
      break;
    case gx::FORWARD_LEFT:
      action.movement = FORWARD_LEFT;
      break;
    case gx::LEFT:
      action.movement = LEFT;
      break;
    case gx::BACKWARD_LEFT:
      action.movement = BACKWARD_LEFT;
      break;
    case gx::BACKWARD:
      action.movement = BACKWARD;
      break;
    case gx::BACKWARD_RIGHT:
      action.movement = BACKWARD_RIGHT;
      break;
    case gx::RIGHT:
      action.movement = RIGHT;
      break;
    case gx::FORWARD_RIGHT:
      action.movement = FORWARD_RIGHT;
      break;
    case gx::NULL_DIR:
      action.movement = NONE;
      break;
  }
  this->sendPacket = true;
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
  std::cout << "Waiting for other players to join" << std::endl;
  while(true) {
    sf::Packet initPacket;
    uint32_t packetType;
    if (netRecv.receiveMessage(initPacket)) {
        initPacket >> packetType;
		IdPacket idp;
		idp.deserialize(initPacket);
		this->id = idp.id;
		this->action.player_id = idp.id; //set the id returned by the id packet
        if (packetType == INIT) break;
    }
  }
  std::cout<<"game started"<<std::endl;
  /*
  //temp code ------------------------------
  s.players[0] =  Player(0,0,1,42);
	s.players[1] = Player(2,3,1,43);
	s.players[2] =  Player(7,2,1,44);
	s.players[3] = Player(4,6,1,45);
  std::vector<std::pair<gx::vector3,int>> entities;
  for(size_t i = 0; i < 4; i++) {
    auto pos = s.players[i].getPosition();
    entities.push_back(std::make_pair(gx::vector3(pos.x,pos.y,pos.z),0));
  }
  gxClient.updateEntities(entities);
  //temp code -----------------------------
  */
  //  main run loop
  while(this->running){
    //process input and send events
    this->processInput(this->gxClient.handleInput());
    this->receiveMessages();
    this->gxClient.draw();
    if(this->sendPacket) {//if dead player still should be able to chat?
      this->action.print();
      this->netRecv.sendPacket<ClientGameTimeAction>(action);
      this->sendPacket = false;
    }
  }
  while(true){}
}
