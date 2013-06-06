#include "NetworkServices.h"
#include "ConfigManager.h"
#ifdef _WIN32
#include <winsock2.h>
//#elif defined(LINUX)
//#include <arpa/inet.h>
#endif

const unsigned short PORT_NUMBER = 55001;
const int TIMEOUT = 2;
const int maxSize = 9000;
const int sizeSize = 4;

ClientServices::ClientServices() {
  //network
  invalidIpAddress = true;
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  std::cout << "Client Ip Address: " << myIpAddress.toString() << std::endl;

  //input is ipaddress to connect to
  std::cout << "Enter Ip Address to connect to: ";
  std::string input = myIpAddress.toString();

  do {
    std::getline(std::cin, input);
    s = sf::Socket::Error;
    s = client.connect(input, PORT_NUMBER, sf::seconds(TIMEOUT));

    if(s == sf::Socket::Done){
      client.setBlocking(false);
      invalidIpAddress = false;
    } else {
      std::cout << "try again, " << input << " is an invalid ip address" << std::endl;
    }
  } while(invalidIpAddress);
}

bool ClientServices::sendMessage(sf::Packet &packet ) {
  return (client.send(packet)==sf::Socket::Done);
}
bool ClientServices::receiveMessage(sf::Packet & packet) {
  auto result = client.receive(packet);
  return result == sf::Socket::Done;
}

  
ServerServices::ServerServices(){
  listener.setBlocking(false);
  listener.listen(PORT_NUMBER); //49152?5535 ports ok to use
  newClient = new sf::TcpSocket; 
}

bool ServerServices::getNewClient(){ 
  // Wait for a connection
  if ( this->listener.accept(*newClient) == sf::Socket::Done ){
    std::cout << "New client connected: " << newClient->getRemoteAddress() << std::endl;
    newClient->setBlocking(false);
    clients.push_back(newClient);
    std::cout << size() << " clients have connceted" << std::endl;
    newClient = new sf::TcpSocket;
    return true;
  }
  return false;
}

bool ServerServices::receiveMessage(sf::Packet &packet, unsigned int i ) {
  if (i < clients.size()) {//error checking for i?
    return (clients[i]->receive(packet) == sf::Socket::Done);
  }
  return false;
}

bool ServerServices::sendMessage(sf::Packet & packet, unsigned int i) {
  if (i < clients.size()) {//error checking for i?
    sf::Uint32 packetSize = packet.getDataSize();
    sf::Uint32 sendSize = htonl(packetSize);
    //maybe I should do some error checking for packet size
    std::vector<char> toSend(packetSize+sizeof(sf::Uint32),0);
    //error checking
    if (packet.getData() && packetSize >0 ){
      std::memcpy(&toSend[0],(char*) &sendSize, sizeof(sf::Uint32));
      std::memcpy(&toSend[sizeof(sf::Uint32)], (char*) packet.getData(), packetSize);
   
      auto result = clients[i]->send(&toSend[0],toSend.size());
      return result == sf::Socket::Done;
    }
  }
  return false;      
}
   
bool ServerServices::sendToAll(sf::Packet & packet ) {
  bool allGood = true;
  for (unsigned int i = 0;i < clients.size();i++) {
    allGood = allGood && this->sendMessage(packet, i);
  }
  return allGood;
}

unsigned int ServerServices::size() {
  return static_cast<unsigned int>(clients.size());
}

ServerServices::~ServerServices() {
  for (int i=0;i<(int)clients.size();i++) {
    delete clients[i];
  }
  delete newClient;
}
