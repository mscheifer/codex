#include "Client.h"

void NetworkClient::chatInit(){
  typing = false;
  chatBuffer = "";
  //chat gfx
  chatFont.loadFromFile("arial.ttf");
  chatText.setFont(chatFont);
  chatText.setColor(sf::Color::Green);
  chatText.setCharacterSize(16);
  chatText.setString("");
  chatText.setPosition(200,200);
}

void NetworkClient::drawChat(sf::RenderWindow& window){
  chatText.setString("");
  for( std::list<std::string>::iterator iter = chatHistory.begin(); iter != chatHistory.end(); iter++){
    chatText.setString(chatText.getString() + "\n" + *iter);
  }
  chatText.setString(chatText.getString() + "\n" + chatBuffer);
  window.draw(chatText);
}

void NetworkClient::sendChatMessage(const std::string& message){
  struct NetworkPacket::Chat netData;
  memcpy( netData.message.data() , message.c_str(), message.length()+1 );
  NetworkPacket netPack;
  netPack.chat = netData;
  netPack.opcode = NetworkPacket::CHAT;
  socket.send(&netPack, sizeof(netPack));
}

void NetworkClient::processMessage(){
  NetworkPacket netPack;
  std::size_t received = 0; 
  sf::TcpSocket::Status stat = socket.receive(&netPack, sizeof(NetworkPacket), received);
  if( stat == sf::Socket::Done ){
    switch(netPack.opcode){
    case NetworkPacket::CHAT:
      recieveChatMessage(netPack);
    }
  }
}

void NetworkClient::recieveChatMessage(const NetworkPacket& netPack){
  char buffer[100];
  memcpy(buffer, &netPack.chat.message, netPack.chat.message.size());
  chatHistory.push_back(buffer);
  if(chatHistory.size() > 10)
    chatHistory.pop_front();
}

void NetworkClient::processInput(sf::Window& window){
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
          if(!typing){ //start typing
            chatBuffer = "";
          }
          else{ //done typing
            if(chatBuffer.size() > 0)
              sendChatMessage(chatBuffer);
              chatBuffer = "";
          }
          typing = !typing;
          break;

        case sf::Keyboard::Back:
          if(typing){
            chatBuffer = chatBuffer.substr(0,chatBuffer.size()-1);
          }
          break;
        }
      }

      if (event.type == sf::Event::TextEntered){
        if( event.text.unicode >= 32 && event.text.unicode <= 125 && typing && chatBuffer.size() < 99)
          chatBuffer += (char) event.text.unicode;
      }
    }
  }
}

void NetworkClient::doClient(){

  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Client Ip Address: " << myIpAddress.toString() << std::endl;

  //input is ipaddress to connect to
  std::cout << "Enter Ip Address to connect to:";
  std::string input = myIpAddress.toString(); //"192.168.1.71";
  s = sf::Socket::Error;
  /*
  //attempt to get ip address
  do{
    std::getline(std::cin, input);

    // Create a socket and connect it to <input ip address> on port 55001
    //socket.setBlocking(false); //TODO remove this
    s = socket.connect(input, 55001, sf::seconds(3.0));

    std::cout << s << std::endl;
    std::cout << "done " << sf::Socket::Done << std::endl;
    std::cout << "not ready " << sf::Socket::NotReady << std::endl;
    std::cout << "dc " << sf::Socket::Disconnected << std::endl;
    std::cout << "err " << sf::Socket::Error << std::endl;

    if(s == sf::Socket::Done ){
      validIpAddress = false;
      socket.setBlocking(false);
    }
    else
      std::cout << "try again, " << input << " is an invalid ip address" << std::endl;

  } while(validIpAddress);
  
  sf::RenderWindow window( sf::VideoMode(800, 600), "sf::Text test" ) ;
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);
  chatInit();

  while(true){
    processInput(window);
    processMessage();

    if(window.isOpen()){

      window.clear();
      if(typing)
        shape.setFillColor(sf::Color::Green);
      else
        shape.setFillColor(sf::Color::Red);
      
      drawChat(window);
      window.draw(shape);
      window.display();
    }
  }

    
    std::string message;
	
    // Send a message to the connected host{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !pressedLeft){

      struct NetworkPacket::Type1 n;
      char m1[] = "asdf";
      char m2[] = "wtf";
      memcpy(&(n.s1), m1, sizeof(*m1)*(std::strlen(m1)+1));
      memcpy(&(n.s2), m2, sizeof(*m2)*(std::strlen(m1)+1));
      NetworkPacket p;
      p.t1 = n;
      p.opcode = NetworkPacket::T1;

      message = "pressed left";
      //std::cout << "sending: " << message << std::endl;
      //socket.send(message.c_str(), message.size() + 1);
      socket.send(&p, sizeof(p));
      pressedLeft = true;
    } else if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      pressedLeft = false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !pressedRight){
      message = "pressed right";
      std::cout << "sending: " << message << std::endl;
      socket.send(message.c_str(), message.size() + 1);
      pressedRight = true;
    } else if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      pressedRight = false;
    }

    // Receive an answer from the server
    char buffer[1024];
    std::size_t received = 0;

    sf::TcpSocket::Status stat = socket.receive(buffer, sizeof(buffer), received);
    if( stat == sf::Socket::Done ){
      std::cout << "The server said: " << buffer << std::endl;
    }

	*/

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
	sf::RenderWindow window( sf::VideoMode(800, 600), "sf::Text test");
	AsciUI as;
	while(window.isOpen()){

		while(window.pollEvent(event)){
	
			c.facingDirection = d; //this should somehow change too;
		

			if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::W) {
					s.players[0].moveForward();//simulating server pending remove
				//	c.forward = true;		
				} if(event.key.code == sf::Keyboard::S) {
					s.players[0].moveBackward(); //simulating server pending remove
				//	c.back = true;
				}if(event.key.code == sf::Keyboard::D) {
					s.players[0].moveRight(); //simulating server pending remove
			//		c.right = true;
				}if(event.key.code == sf::Keyboard::A) {
					s.players[0].moveLeft(); //simulating server pending remove
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