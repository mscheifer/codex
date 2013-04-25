#include "NetworkServices.h"

Opcode processMeta(sf::Packet & packet) {
  size_t packetMeta;
  packet >> packetMeta;
//  bool initGame = packetMeta >> 31; 
//  bool error = packetMeta & 0x1;
  if (packetMeta == ERROR_NET) return ERROR;
  if (packetMeta == INIT_NET) {std::cout<<"received init game"<<std::endl; return INIT;} 
  if (packetMeta == CHAT_NET) return CHAT;
  return ERROR;
}

ClientServices::ClientServices(){
    //network
    invalidIpAddress = true;
    sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
    std::cout << "Client Ip Address: " << myIpAddress.toString() << std::endl;

    //input is ipaddress to connect to
    //std::cout << "Enter Ip Address to connect to:";
    std::string input = myIpAddress.toString(); //"192.168.1.71";

    //TODO uncomment this
    //do{
    //std::getline(std::cin, input);
    s = sf::Socket::Error;
    s = client.connect(input, PORT_NUMBER, sf::seconds(TIMEOUT));
    
     if(s == sf::Socket::Done){
       client.setBlocking(false);
       invalidIpAddress = false;
     }
     else
       std::cout << "try again, " << input << " is an invalid ip address" << std::endl;

    //}while(invalidIpAddress);
  }
  bool ClientServices::sendMessage(sf::Packet &packet ) {
    return (client.send(packet)==sf::Socket::Done);
  }
  bool ClientServices::receiveMessage(sf::Packet & packet) {
    return (client.receive(packet)==sf::Socket::Done);
  }

  
  ServerServices::ServerServices(){
    listener.setBlocking(false);
    listener.listen(PORT_NUMBER); //49152–65535 ports ok to use
    newClient = new sf::TcpSocket; 
  }

   void ServerServices::getNewClient(){ 
  // Wait for a connection
    if ( listener.accept(*newClient) == sf::Socket::Done ){
      std::cout << "New client connected: " << newClient->getRemoteAddress() << std::endl;
      newClient->setBlocking(false);
      clients.push_back(newClient);
      std::cout <<size()<<" clients have connceted"<<std::endl;
      newClient = new sf::TcpSocket;
    }
   }

   bool ServerServices::receiveMessage(sf::Packet &packet, int i ) {
      if (i>=0 && i<(int)clients.size())//error checking for i?
        return (clients[i]->receive(packet)==sf::Socket::Done);
      return false;
   }

   bool ServerServices::sendMessage(sf::Packet & packet, int i) {
      if (i>=0 && i<(int)clients.size())//error checking for i?
        return (clients[i]->send(packet)==sf::Socket::Done);
      return false;      
   }
   
   void ServerServices::sendToAll(sf::Packet & packet ) {
     for (int i=0;i<(int)clients.size();i++) {
       sendMessage(packet, i);
     }
   }

   int ServerServices::size() {
     return clients.size();
   }

   ServerServices::~ServerServices() {
     for (int i=0;i<(int)clients.size();i++) {
        delete clients[i];
     }
     delete newClient;
   }