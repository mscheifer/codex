#pragma once
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include <iostream>
#include <string>
#include <list>
#include "NetworkPacket.h"
#include "AsciUI.h"
#include "ServerGameTimeRespond.h"

class NetworkClient{

private:
  //network vars
  sf::TcpSocket socket;
  sf::Socket::Status s;
  bool validIpAddress;
  
  bool pressedLeft;
  bool pressedRight;
  
  //chat vars
  std::list<std::string> chatHistory;
  std::string chatBuffer;
  sf::Text chatText;
  sf::Font chatFont;
  bool typing;

public:
  NetworkClient(){
    //network
    validIpAddress = true;

    pressedLeft = false;
    pressedRight = false;

    //chat
    chatInit();
  }

  ~NetworkClient(){
  }

  void processInput(sf::Window& window);

  //chat functions
  //initialize chat variables
  void chatInit();
  
  //draw the chat
  void drawChat(sf::RenderWindow& window);

  //send a chat message over the network
  void sendChatMessage(const std::string& message);
  
  //recieve a chat message over the network
  void recieveChatMessage(const NetworkPacket& netPack);
  
  //recieve a general networked message
  void processMessage();

  void doClient();
};