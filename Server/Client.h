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

class NetworkClient{
private:
  AsciUI as; //TODO delete this later
  ServerGameTimeRespond s;
	ClientGameTimeAction action;
  ClientServices netRecv;
  sf::RenderWindow window;
  ChatHandler chat;

  bool sendPacket;

  //this is for collision detection test
  sf::CircleShape c1;
  boundingSphere s1;
  sf::CircleShape c2;
  boundingSphere s2;
  //end collision

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
