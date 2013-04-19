#include "Client.h"

void NetworkClient::receiveMessages() {
      //receive from server and process
      sf::Packet packet;
      if (netRecv.receiveMessage(packet)) {
        int32_t packetType =0;
        std::string chatMsg;
        packet >> packetType;
        switch (packetType) {
        case CHAT:
          packet >>chatMsg;
          chat.addChat(chatMsg);
          break;
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
  if (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
          window.close();

      if (event.type == sf::Event::KeyReleased){        
        switch(event.key.code){
        case sf::Keyboard::Return:
          if(!chat.isTyping()){ //start typing
            chat.setBuffer("");
          }
          else{ //done typing
            sf::Packet packet;
            std:: string chatBuffer = chat.getBuffer();
            //when sending packet
            //first specify the packet type
            //then call serailize on the sending object`
            packet << CHAT;
            packet << chat.getBuffer();
            if(chatBuffer.size() > 0)
              netRecv.sendMessage(packet);
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
}

void NetworkClient::doClient(){
    while(true){
      sf::Packet packet;
      //process input and send events
      processInput(); 

      receiveMessages();

      updateWindow();
    }
    
/*    
    struct ServerGameTimeRespond s;
    s.players[0] =  Player(0,0,1);
    s.players[1] = Player(2,3,1);
    s.players[2] =  Player(7,2,1);
    s.players[3] = Player(4,6,1);

    sf::Event event;
    sf::RenderWindow window( sf::VideoMode(800, 600), "sf::Text test");
    AsciUI as;
    while(window.isOpen()){
        while(window.pollEvent(event)){
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::W) {
                    s.players[0].moveForward();
                    as.render(s.players);
                } else if(event.key.code == sf::Keyboard::S) {
                    s.players[0].moveBackward();
                    as.render(s.players);
                }else if(event.key.code == sf::Keyboard::D) {
                    s.players[0].moveRight();
                    as.render(s.players);
                }else if(event.key.code == sf::Keyboard::A) {
                    s.players[0].moveLeft();
                    as.render(s.players);
        }else if(event.key.code == sf::Keyboard::Space) {
                    s.players[0].jump();
                    as.render(s.players);
                }
            
            }

        }
    }
  */
}