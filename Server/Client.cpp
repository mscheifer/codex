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
        if( event.text.unicode >= 32 && event.text.unicode <= 125 && typing && chatText.getString().getSize() < 99)
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
  /*
  //attempt to get ip address
  do{
    //std::getline(std::cin, input);

    // Create a socket and connect it to <input ip address> on port 55001
    socket.setBlocking(false); //TODO remove this
    s = socket.connect(input, 55001);

    if(s != sf::Socket::Error)
      validIpAddress = false;
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
				}
			
			}

		}
	}
  
}