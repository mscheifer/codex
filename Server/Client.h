#ifndef CLIENT_H
#define CLIENT_H
#include "NetworkServices.h"
#include "graphics/graphicsClient.h"
#include "ServerGameTimeRespond.h"
#include "ChatHandler.h"
#include "EntityPool.h"

class NetworkClient {
  ServerGameTimeRespond s;
  ClientGameTimeAction action;
  ClientServices netRecv;
  ChatHandler chat;
  gx::graphicsClient gxClient;
  int id;
  bool sendPacket;
  bool running;
  EntityPool objPool;
  Entity skybox; //this is kind of stupid but we'll do it for now

  void processInput();
  void receiveMessages();
public:
  NetworkClient(): s(&objPool), action(), netRecv(), chat(), gxClient(), id(-1),
                   sendPacket(false), running(true) {
    skybox.setPosition(gx::vector3f(0,0,0));
    skybox.setDirection(gx::vector3f(0,1,0));
  }
  NetworkClient(const NetworkClient&);// = delete;
  NetworkClient& operator=(const NetworkClient&);// = delete;
  NetworkClient(NetworkClient&&);// = delete;
  NetworkClient& operator=(NetworkClient&&);// = delete;
  void doClient();
};
#endif //CLIENT_H
