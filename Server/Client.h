#pragma once
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include "NetworkPacket.h"
#include "NetworkServices.h"
#include "AsciUI.h"
#include "ServerGameTimeRespond.h"
#include "ChatHandler.h"

#define L_ASCII_BOUND 32
#define U_ASCII_BOUND 125


class NetworkClient{

private:
  ClientServices netRecv;
  sf::RenderWindow window;
  ChatHandler chat;

public:
  NetworkClient():window(sf::VideoMode(800,600), "sf::Text test")
  {
  }

  ~NetworkClient(){
  }
  void processInput();
  void updateWindow();
  void receiveMessages();
  //void processInput(sf::Window& window);

  //chat functions
  //initialize chat variables
  
  //draw the chat

  //send a chat message over the network
 // void sendChatMessage(const std::string& message);
  
  //recieve a chat message over the network
 // void recieveChatMessage(const NetworkPacket& netPack);
  
  //recieve a general networked message
//  void processMessage();

  void doClient();
};