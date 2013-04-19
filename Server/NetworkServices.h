#pragma once
#include <stdint.h>
#include <iostream>
#include <SFML/Network.hpp>
#define PORT_NUMBER 55001
#define TIMEOUT 3

enum Opcode {INIT, CHAT , T1, T2};

const int maxSize = 9000;
const int sizeSize = 4;

template <typename Data>
void sendPacket(Data & data) {
  sf::Packet packet;
  packet.clear();
  packet << CHAT; //data.packetType
  data.serialize(packet);
  sendMessage(packet); 
}

class ClientServices{
public:
  //network vars
  sf::Socket::Status s;
  sf::TcpSocket client;
  bool validIpAddress;
  
  ClientServices();
  bool sendMessage(sf::Packet &packet );
  bool receiveMessage(sf::Packet & packet);
};

class ServerServices{
public:
  sf::TcpListener listener;
  std::vector<sf::TcpSocket*> clients;
  sf::TcpSocket * newClient;
  
   ServerServices();
   void getNewClient(); 
   bool receiveMessage(sf::Packet &packet, int i );
   bool sendMessage(sf::Packet & packet, int i);
   void sendToAll(sf::Packet & packet );
   int size();
   ~ServerServices();
};