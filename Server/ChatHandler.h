#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <SFML/Network.hpp>
#include "NetworkServices.h" //just for DEFINE right now

class ChatHandler {
public:
  std::list<std::string> chatHistory;
  std::string chatBuffer;
  sf::Text chatText;
  sf::Font chatFont;
  bool typing;    
  sf::CircleShape shape;
  ChatHandler(void);
  ~ChatHandler(void);
  void drawChat(sf::RenderWindow & window);
  void addChat(std::string & chatMsg);
  void backspace();
  bool isTyping();
  void setBuffer(const std::string & buffer );
  std::string getBuffer();
  void revertTyping();
  void appendBuffer(char c);
};

class ChatObject {
public:
  std::string chatMsg;
  size_t packetType;
  ChatObject():chatMsg("") {packetType = CHAT_NET;}
  ChatObject(std::string chatMsg):chatMsg(chatMsg) {packetType = CHAT_NET;}
  void serialize(sf::Packet & packet);
  void deserialize(sf::Packet & packet);
  std::string getChat() { return chatMsg;}
};
