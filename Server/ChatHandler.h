#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>

class ChatHandler
{
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