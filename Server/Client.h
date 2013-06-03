#ifndef CLIENT_H
#define CLIENT_H
#include "NetworkServices.h"
#include "graphics/graphicsClient.h"
#include "ChatHandler.h"
#include "EntityPool.h"
#include "AudioManager.h"
#include "Game.h"
#include <iostream>

class NetworkClient {
  ServerGameTimeRespond s;
  ClientGameTimeAction action;
  ClientServices netRecv;
  ChatHandler chat;
  gx::graphicsClient gxClient;
  int id;
  bool sendPacket;
  bool running;
  bool gameRestart;
  bool joined;
  bool gameStart;
  EntityPool objPool;

  void processInput();
  void receiveMessages();
public:
  NetworkClient(): s(&objPool), action(), netRecv(), chat(), gxClient(), id(-1),
                   sendPacket(false), running(true), gameRestart(false), joined(false),gameStart(false){
  }
  NetworkClient(const NetworkClient&);// = delete;
  NetworkClient& operator=(const NetworkClient&);// = delete;
  NetworkClient(NetworkClient&&);// = delete;
  NetworkClient& operator=(NetworkClient&&);// = delete;
  void doClient();
};
#endif //CLIENT_H
