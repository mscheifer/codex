#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <SFML/Network.hpp>
#include "StaticEnums.h" //just for DEFINE right now

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
  void addChat(const std::string & chatMsg);
  void backspace();
  bool isTyping();
  void setBuffer(const std::string & buffer );
  std::string getBuffer();
  void revertTyping();
  void appendBuffer(char c);
};

class ChatObject {
public:
  static const int packetType = CHAT;  //might be a BUG
  std::string chatMsg;
  ChatObject():chatMsg("") {}
  ChatObject(std::string chatMsg):chatMsg(chatMsg) {}
  void serialize(sf::Packet & packet);
  void deserialize(sf::Packet & packet);
  std::string getChat() { return chatMsg;}
};
