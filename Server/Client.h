//#pragma once
#ifndef CLIENT
#define CLIENT
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include "NetworkServices.h"
#include "AsciUI.h"
#include "ServerGameTimeRespond.h"
#include "ChatHandler.h"

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
  void doClient();
};
#endif