#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>


sf::Text chatText;
sf::Font chatFont;
bool typing;

void chatInit(){
  typing = false;
  chatFont.loadFromFile("arial.ttf");
  chatText.setFont(chatFont);
  chatText.setColor(sf::Color::Green);
  chatText.setCharacterSize(16);
  chatText.setString("asdf\nasdf");
  chatText.setPosition(200,200);
}

////////////////////////////CLIENT//////////////////////////////
////////////////////////////////////////////////////////////////
int main(){
    sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
    std::cout << "Client Ip Address: " << myIpAddress.toString() << std::endl;

    //input is ipaddress to connect to
    std::cout << "Enter Ip Address to connect to:";
    std::string input = myIpAddress.toString(); //"192.168.1.71";
    sf::TcpSocket socket;
    bool validIpAddress = true;
    bool pressedLeft = false;
    bool pressedRight = false;
    sf::Socket::Status s;

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
                    chatText.setString("");
                  }
                  else{ //done typing
                    if(chatText.getString().getSize() > 0)
                      std::cout << chatText.getString().toAnsiString() << std::endl;
                  }
                  typing = !typing;
                  break;

                case sf::Keyboard::Back:
                  if(typing){
                    chatText.setString( chatText.getString().toAnsiString().substr(0,chatText.getString().getSize()-1) );
                  }
                  break;
                }
              }

              if (event.type == sf::Event::TextEntered){
                if( event.text.unicode >= 32 && event.text.unicode <= 125 && typing && chatText.getString().getSize() < 99)
                  chatText.setString( chatText.getString() + (char) event.text.unicode);
              }
          }

          window.clear();
          if(typing)
            shape.setFillColor(sf::Color::Green);
          else
            shape.setFillColor(sf::Color::Red);
          window.draw(shape);
          window.draw(chatText);
          window.display();
      }

      std::string message;

      // Send a message to the connected host{
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !pressedLeft){
        message = "pressed left";
        std::cout << "sending: " << message << std::endl;
        socket.send(message.c_str(), message.size() + 1);
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
    }

    std::getline(std::cin, input);
    std::cout << input << std::endl;
    std::getline(std::cin, input);
    return 0;
}