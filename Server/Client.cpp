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
/*
void NetworkClient::sendChatMessage(const std::string& message){
  struct NetworkPacket::Chat netData;
  memcpy( netData.message.data() , message.c_str(), message.length()+1 );

  NetworkPacket netPack;
  netPack.chat = netData;
  netPack.opcode = NetworkPacket::CHAT;
  socket.send(&netPack, sizeof(netPack));
}
*/
/*
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
} */
/*
void NetworkClient::recieveChatMessage(const NetworkPacket& netPack){
  char buffer[100];
  memcpy(buffer, &netPack.chat.message, netPack.chat.message.size());
  chatHistory.push_back(buffer);
  if(chatHistory.size() > 10)
    chatHistory.pop_front();
}
*/
/*
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
*/
void NetworkClient::doClient(){

    ClientServices netRecv;
    //get current time
    sf::Packet packet;
    
    clock_t c = std::clock();
    std::stringstream out;
    out << c;
    std::string str = out.str();
    
    //send first packet
    std::cout << "start send" << str << std::endl;
    
    //specify packet type 
    packet<<CHAT;
    packet<<str;
   
    //socket.send(&netPack, sizeof(netPack));
    netRecv.sendMessage(packet);

    //keep sending and calculate difference in time
    while(true){
      sf::Packet packet;
      if (netRecv.receiveMessage(packet)) {
        clock_t end = std::clock();
        int32_t packetType;
        packet >> packetType; 
        std::string startTime;
        packet >>startTime;
        clock_t start = std::atoi(startTime.c_str());
        std::cout << "start " << start << " end " << end << " " << (float) (end - start)/CLOCKS_PER_SEC*1000 << " ms" << std::endl;
      
        //send new one
        packet.clear();
        std::stringstream out;
        std::string str;
        out << std::clock();
        str = out.str();
        packet <<CHAT;
        packet <<str;
        std::cout << "new start " << out.str() << std::endl;
        netRecv.sendMessage(packet);
      }
    }
   
    
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
  
}