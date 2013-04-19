#pragma once

#include <stdint.h>
#include <SFML/Network.hpp>
#include "NetworkPacket.h"
#include <algorithm>
#define PORT_NUMBER 55001
#define TIMEOUT 3

enum Opcode {INIT, CHAT , T1, T2};

const int maxSize = 9000;
const int sizeSize = 4;
 
class ClientServices{
public:
  //network vars
  sf::Socket::Status s;
  sf::TcpSocket client;
  bool validIpAddress;
  
  ClientServices(){
    //network
    validIpAddress = true;
    
    sf::IpAddress myIpAddress = sf::IpAddress::getLocalAddress();
    std::cout << "Client Ip Address: " << myIpAddress.toString() << std::endl;

    //input is ipaddress to connect to
    std::cout << "Enter Ip Address to connect to:";
    std::string input = myIpAddress.toString(); //"192.168.1.71";
    s = sf::Socket::Error;
    s = client.connect(input, PORT_NUMBER, sf::seconds(TIMEOUT));
    client.setBlocking(false);
  //change  this->socket = socket;
  }
  bool sendMessage(sf::Packet &packet ) {
    return (client.send(packet)==sf::Socket::Done);
  }
  bool receiveMessage(sf::Packet & packet) {
    return (client.receive(packet)==sf::Socket::Done);
  }
};

class ServerServices{
public:
  sf::TcpListener listener;
  std::vector<sf::TcpSocket*> clients;
  sf::TcpSocket * newClient;
  
   ServerServices(){
    listener.setBlocking(false);
    listener.listen(55001); //49152–65535 ports ok to use
    newClient = new sf::TcpSocket; 
  }
   void getNewClient(){ 
  // Wait for a connection
    if ( listener.accept(*newClient) == sf::Socket::Done ){
      std::cout << "New client connected: " << newClient->getRemoteAddress() << std::endl;
      newClient->setBlocking(false);
      clients.push_back(newClient);
      newClient = new sf::TcpSocket;
    }
   }
   bool receiveMessage(sf::Packet &packet, int i ) {
      if (i>=0 && i<clients.size())//error checking for i?
        return (clients[i]->receive(packet)==sf::Socket::Done);
      return false;
   }
   bool sendMessage(sf::Packet & packet, int i) {
      if (i>=0 && i<clients.size())//error checking for i?
        return (clients[i]->send(packet)==sf::Socket::Done);
      return false;      
   }
   void sendToAll(sf::Packet & packet ) {
     for (int i=0;i<clients.size();i++) {
       sendMessage(packet, i);
     }
   }
   int size() {
     return clients.size();
   }
   ~ServerServices() {
     for (int i=0;i<clients.size();i++) {
        delete clients[i];
     }
     delete newClient;
   }
};