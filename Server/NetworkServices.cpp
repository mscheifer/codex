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
  //code moved to connectServer
}

bool ClientServices::connectServer(std::string serverIP) {
  sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
  //std::cout << "Client Ip Address: " << myIpAddress.toString() << std::endl;

  //input is ipaddress to connect to
  std::cout << "Trying to connect to "<<serverIP<<std::endl;
  std::string input = (serverIP.compare("")) ? serverIP : myIpAddress.toString();
  s = sf::Socket::Error;
  s = client.connect(input, PORT_NUMBER, sf::seconds(TIMEOUT));

  if(s == sf::Socket::Done){
    client.setBlocking(false);
    return true;
  } 
  std::cout<<"there is an error connecting to the server\n";
  return false;
}

bool ClientServices::sendMessage(sf::Packet &packet ) {
  sf::Uint32 packetSize = packet.getDataSize();
  sf::Uint32 sendSize = htonl(packetSize);
  std::vector<char> toSend(packetSize+sizeof(sf::Uint32),0);
  //error checking
  if (packet.getData() && packetSize >0 ){
    std::memcpy(&toSend[0],(char*) &sendSize, sizeof(sf::Uint32));
    std::memcpy(&toSend[sizeof(sf::Uint32)], (char*) packet.getData(), packetSize);
 
    auto result = client.send(&toSend[0],toSend.size());
    return result == sf::Socket::Done;
  }
  return false; 
}

std::string toHex(unsigned char oneChar) {
  char mapping[16] =  {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  unsigned char one = (oneChar & 0xF0) >>4;
  unsigned char two = oneChar & 0x0F;
  return std::string(&mapping[one],1) +std::string(&mapping[two],1);
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
