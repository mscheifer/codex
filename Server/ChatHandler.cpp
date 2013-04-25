#include "ChatHandler.h"

void ChatHandler::addChat (std::string & chatMsg) {
  chatHistory.push_back(chatMsg);
  if (chatHistory.size()>10)
    chatHistory.pop_front();
}

ChatHandler::ChatHandler(void): shape(10.f)
{
  typing = false;
  chatBuffer = "";
  //chat gfx
  chatFont.loadFromFile("arial.ttf");
  chatText.setFont(chatFont);
  chatText.setColor(sf::Color::Green);
  chatText.setCharacterSize(16);
  chatText.setString("");
  chatText.setPosition(200,200);
  shape.setFillColor(sf::Color::Green);

}
void ChatHandler:: backspace() {
 if (chatBuffer.size())
   chatBuffer = chatBuffer.substr(0,chatBuffer.size()-1);
}

bool ChatHandler::isTyping() {
  return typing;
}

void ChatHandler::setBuffer(const std::string &buffer ) {
  chatBuffer = buffer;
}

void ChatHandler::revertTyping() {
  typing = !typing;
}
std::string ChatHandler::getBuffer() {
  return chatBuffer;
}
void ChatHandler::appendBuffer(char c ) {
  chatBuffer += c;
}

void ChatHandler::drawChat(sf::RenderWindow& window){
  if(typing)
    shape.setFillColor(sf::Color::Green);
  else
    shape.setFillColor(sf::Color::Red); 
  window.draw(shape);
  chatText.setString("");
  for( std::list<std::string>::iterator iter = chatHistory.begin(); iter != chatHistory.end(); iter++){
    chatText.setString(chatText.getString() + "\n" + *iter);
  }
  chatText.setString(chatText.getString() + "\n" + chatBuffer);
  window.draw(chatText);
}

ChatHandler::~ChatHandler(void)
{
}
