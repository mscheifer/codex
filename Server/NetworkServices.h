//#pragma once
#ifndef NETWORKSERVICE
#define NETWORKSERVICE
#include <stdint.h>
#include <iostream>
#include <SFML/Network.hpp>
#include "StaticEnums.h"
#define PORT_NUMBER 55001
#define TIMEOUT 1
#define NUM_PLAYERS 2

const int maxSize = 9000;
const int sizeSize = 4;

class ClientServices{
public:
  //network vars
  sf::Socket::Status s;
  sf::TcpSocket client;
  bool invalidIpAddress;
  
  ClientServices();
  bool sendMessage(sf::Packet &packet );
  bool receiveMessage(sf::Packet & packet);
  
  //we might need to put this in cpp. might have compile problems
  template <typename Data>
  void sendPacket(Data & data) {
    sf::Packet packet;
    packet.clear();
    packet << Data::packetType; 
    data.serialize(packet);
    sendMessage(packet); 
  }
};

class ServerServices{
public:
  sf::TcpListener listener;
  std::vector<sf::TcpSocket*> clients;
  sf::TcpSocket * newClient;
  
   ServerServices();
   template <typename Data>
   void sendPacketToAll(Data & data) {
     sf::Packet packet;
     packet.clear();
     packet << Data::packetType;
     data.serialize(packet);
     sendToAll(packet); 
   }
   
   template <typename Data>
   void sendPacket(Data & data, int i) {
     sf::Packet packet;
     packet.clear();
     packet << Data::packetType;
     data.serialize(packet);
     sendMessage(packet, i); 
   }

   void getNewClient(); 
   bool receiveMessage(sf::Packet &packet, int i );
   bool sendMessage(sf::Packet & packet, int i);
   void sendToAll(sf::Packet & packet );
   int size();
   ~ServerServices();
};
#endif