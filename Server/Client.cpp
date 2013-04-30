#include "Client.h"

void NetworkClient::receiveMessages() {
      //receive from server and process
      sf::Packet packet;
      if (netRecv.receiveMessage(packet)) {
        ChatObject chatObj;
        size_t packetType;
        IdPacket newId(0);
        packet >> packetType;
        switch (packetType) {
        case CHAT:
          chatObj.deserialize(packet);
          chat.addChat(chatObj.getChat());
          break;
        case SGTR:
          s.deserialize(packet);
          as.render(s.players);
          if (s.players[id].getHealth() == 0) alive = false;
          //render death
          break;
        case JOINID:
          newId.deserialize(packet);
          id = newId.id;
          std::cout<<"USERID:"<<id<<std::endl;
          action.player_id = id;
          break;
        case WIN:
          //render win
          break;
        case LOSE:
          //render lose 
        default: 
          break;
        }
      }
}
void NetworkClient::updateWindow() {
  if (window.isOpen()) {
    window.clear();
    chat.drawChat(window);
    window.display();
  }
}

void NetworkClient::processInput(){
  bool updateAS = false;
  action.clear();
  
  if (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event)) {

      if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::W) {
          action.movement = FORWARD;
          updateAS = true;
				} if(event.key.code == sf::Keyboard::S) {
          if(action.movement == FORWARD)
            action.movement = NONE;
          else
            action.movement = BACKWARD;
          updateAS = true;
				}if(event.key.code == sf::Keyboard::D) {
          if(action.movement == FORWARD)
            action.movement = FORWARD_RIGHT;
          else if (action.movement == BACKWARD)
            action.movement = BACKWARD_RIGHT;
          else
            action.movement = RIGHT;
          updateAS = true;
				}if(event.key.code == sf::Keyboard::A) {
          if(action.movement == FORWARD)
            action.movement = FORWARD_LEFT;
          else if (action.movement == BACKWARD)
            action.movement = BACKWARD_LEFT;
          else if (action.movement == RIGHT)
            action.movement = NONE;
          else
            action.movement = LEFT;
          updateAS = true;
				}if(event.key.code == sf::Keyboard::Space) {
          action.jump = true;
          updateAS = true;
        }
      }

      if (event.type == sf::Event::Closed)
          window.close();

      if (event.type == sf::Event::KeyReleased){        
        switch(event.key.code){
        case sf::Keyboard::Right:
          c1.setPosition(c1.getPosition().x+5 , c1.getPosition().y);
          s1.move(gx::vector3(5,0,0));
          break;
        case sf::Keyboard::Left:
          c1.setPosition(c1.getPosition().x-5 , c1.getPosition().y);
          s1.move(gx::vector3(-5,0,0));
          break;
        case sf::Keyboard::Up:
          c1.setPosition(c1.getPosition().x , c1.getPosition().y-5);
          s1.move(gx::vector3(0,0,-5));
          break;
        case sf::Keyboard::Down:
          c1.setPosition(c1.getPosition().x , c1.getPosition().y+5);
          s1.move(gx::vector3(0,0,5));
          break;
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

  if(updateAS && alive){ //player should still be able to chat?
    sendPacket = true;
  }
}

void NetworkClient::doClient(){
  /*
  std::cout<<"Waiting for other players to join"<<std::endl;
  while(true) {
    sf::Packet initPacket;
    size_t packetType;
    if (netRecv.receiveMessage(initPacket)) {
        initPacket >> packetType;
        if (packetType==INIT) break;
    }
  }
  std::cout<<"game started"<<std::endl;
  */

  s.players[0] =  Player(0,0,1,42);
	s.players[1] = Player(2,3,1,43);
	s.players[2] =  Player(7,2,1,44);
	s.players[3] = Player(4,6,1,45);
  as.render(s.players);

  //  main run loop
  while(true){
    //process input and send events
    processInput(); 
    receiveMessages();
    updateWindow();
    if(sendPacket){ //if dead player still should be able to chat?
      action.print();
      netRecv.sendPacket<ClientGameTimeAction>(action);
      sendPacket = false;
    }
  }
}