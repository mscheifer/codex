#include "Client.h"

void NetworkClient::doClient(){
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

  while(true){
    std::string message;

	/*
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
	int x;
	Map* map = new Map();
	AsciUI* as = new AsciUI();
	as->update(map);
	std::cin >> x;
  }
}