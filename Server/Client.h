#ifndef CLIENT_H
#define CLIENT_H
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include <iostream>
#include <string>
#include <list>
#include "NetworkServices.h"
#include "graphics/graphicsClient.h"
#include "ServerGameTimeRespond.h"
#include "ChatHandler.h"
#include "boundingSphere.h"
#include "Game.h"
#include "Quadtree.h"

class NetworkClient {
  ServerGameTimeRespond s;
  ClientGameTimeAction action;
  ClientServices netRecv;
  ChatHandler chat;
  gx::graphicsClient gxClient;
  int id;
  bool sendPacket;
  bool running;

public:
  NetworkClient(): s(), action(), netRecv(), chat(), gxClient(), id(-1), sendPacket(false), running(true) {}
  NetworkClient(const NetworkClient&);// = delete;
  NetworkClient& operator=(const NetworkClient&);// = delete;
  NetworkClient(NetworkClient&&);// = delete;
  NetworkClient& operator=(NetworkClient&&);// = delete;
  void processInput(gx::userInput);
  void receiveMessages();
  void doClient();
};
#endif //CLIENT_H
