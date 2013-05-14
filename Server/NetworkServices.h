#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H
#include <SFML/Network.hpp>

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

class ServerServices {
  public:
    sf::TcpListener listener;
    std::vector<sf::TcpSocket*> clients;
    sf::TcpSocket * newClient;
  
   ServerServices();
   ~ServerServices();
   template <typename Data>
   bool sendPacketToAll(const Data & data) {
     sf::Packet packet;
     packet.clear();
     packet << Data::packetType;
     data.serialize(packet);
     return sendToAll(packet); 
   }
   
   template <typename Data>
   bool sendPacket(const Data& data, unsigned int i) {
     sf::Packet packet;
     packet.clear();
     packet << Data::packetType;
     data.serialize(packet);
     return sendMessage(packet, i); 
   }

   bool getNewClient(); 
   bool receiveMessage(sf::Packet &packet, unsigned int i );
   bool sendMessage(sf::Packet & packet, unsigned int i);
   bool sendToAll(sf::Packet & packet );
   unsigned int size();
};
#endif //NETWORKSERVICE_H
