#include "Client.h"

void NetworkClient::receiveMessages() {
      //receive from server and process
      sf::Packet packet;
      if (netRecv.receiveMessage(packet)) {
        ChatObject chatObj;
        size_t packetType;
        packet >> packetType;
        switch (packetType) {
        case CHAT:
          chatObj.deserialize(packet);
          chat.addChat(chatObj.getChat());
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
}

void NetworkClient::doClient(){
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
  while(true){
    //process input and send events
    processInput(); 
    receiveMessages();
    updateWindow();
  }
 
  ClientGameTimeAction test1;
  test1.player_id = 9;
  test1.movement = LEFT;
  test1.attackMelee = true;
  test1.attackRange = false;
  test1.weapon1 = true;
  test1.weapon2 = false;
  test1.jump = true;
  test1.facingDirection = Direction(1,2,3);
  netRecv.sendPacket<ClientGameTimeAction>( test1 );
    

	//temp code to simular server respond
    struct ServerGameTimeRespond s;
	s.players[0] =  Player(0,0,1,42);
	s.players[1] = Player(2,3,1,43);
	s.players[2] =  Player(7,2,1,44);
	s.players[3] = Player(4,6,1,45);
	//end of temp code

	ClientGameTimeAction c ;
	Direction d; 
    sf::Event event;
    //sf::RenderWindow window( sf::VideoMode(800, 600), "sf::Text test");
    AsciUI as;
    while(window.isOpen()){

        while(window.pollEvent(event)){
	
			c.facingDirection = d; //this should somehow change too;
		

       if(event.type == sf::Event::KeyPressed){
          if(event.key.code == sf::Keyboard::W) {
					s.players[0].moveTowardDirection(FORWARD);//simulating server pending remove
				//	c.forward = true;		
				} if(event.key.code == sf::Keyboard::S) {
					s.players[0].moveTowardDirection(BACKWARD); //simulating server pending remove
				//	c.back = true;
				}if(event.key.code == sf::Keyboard::D) {
					s.players[0].moveTowardDirection(RIGHT); //simulating server pending remove
			//		c.right = true;
				}if(event.key.code == sf::Keyboard::A) {
					s.players[0].moveTowardDirection(LEFT); //simulating server pending remove
			//		c.left = true;
				}if(event.key.code == sf::Keyboard::Space) {
					s.players[0].jump(); //simulating server pending remove
					c.jump = true;
                }
				//send client run time c
				//render s
				as.render(s.players);
            
            }

        }
    }
  
}