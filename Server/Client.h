#ifndef CLIENT_H
#define CLIENT_H
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Weapon.h"
#include <iostream>
#include <string>
#include <list>
#include "NetworkServices.h"
#include "graphics/graphicsClient.h"
#include "ServerGameTimeRespond.h"
#include "ChatHandler.h"
#include "Game.h"
#include "ConfigManager.h"
#include "AudioManager.h"

class NetworkClient {
  ServerGameTimeRespond s;
  ClientGameTimeAction action;
  ClientServices netRecv;
  ChatHandler chat;
  gx::graphicsClient gxClient;
  int id;
  bool sendPacket;
  bool running;

  void processInput();
  void receiveMessages();
public:
  NetworkClient(): s(), action(), netRecv(), chat(), gxClient(), id(-1),
                   sendPacket(false), running(true) {}
  NetworkClient(const NetworkClient&);// = delete;
  NetworkClient& operator=(const NetworkClient&);// = delete;
  NetworkClient(NetworkClient&&);// = delete;
  NetworkClient& operator=(NetworkClient&&);// = delete;
  void doClient();
};
#endif //CLIENT_H
