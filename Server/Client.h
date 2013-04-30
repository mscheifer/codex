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

class NetworkClient{
private:
  bool alive;
  AsciUI as; //TODO delete this later
  ServerGameTimeRespond s;
	ClientGameTimeAction action;
  ClientServices netRecv;
  sf::RenderWindow window;
  ChatHandler chat;
  int id;
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
    alive=true;
  }

  ~NetworkClient(){
  }
  void processInput();
  void updateWindow();
  void receiveMessages();
  void doClient();
};
#endif
