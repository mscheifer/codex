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
#include "boundingSphere.h"
#include "Game.h"
#include "Quadtree.h"

class NetworkClient{
private:
  AsciUI as; //TODO delete this later
  ServerGameTimeRespond s;
	ClientGameTimeAction action;
  ClientServices netRecv;
  sf::RenderWindow window;
  ChatHandler chat;
  int id;
  bool sendPacket;

public:
  NetworkClient():window(sf::VideoMode(800,600), "sf::Text test")
  {
    sendPacket = false;
  }

  ~NetworkClient(){
  }
  void processInput();
  void updateWindow();
  void receiveMessages();
  void doClient();
};
#endif
