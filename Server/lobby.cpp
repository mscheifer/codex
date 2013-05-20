#include "lobby.h"
#include <iostream>

lobby::lobby(void):start(false)  {
  font.loadFromFile("arial.ttf");
  welcome.setFont(font);
  welcome.setColor(sf::Color::Red);
  welcome.setCharacterSize(36);
  welcome.setString("Please click join to start.");
  welcome.setPosition(200,200);
  buttonRect.setSize(sf::Vector2f(250,45));
  buttonRect.setOutlineColor(sf::Color::Red);
  buttonRect.setOutlineThickness(2);
  buttonRect.setPosition(280,300);
  button.setFont(font);
  button.setColor(sf::Color(150,50,50));
  button.setCharacterSize(36);
  button.setString("join the game");
  buttonBounds = buttonRect.getGlobalBounds();
  sf::FloatRect textBounds = button.getGlobalBounds();
  button.setPosition(280+(buttonBounds.width-textBounds.width)/2,300);
}


lobby::~lobby(void) {
}

void lobby::handleInput(sf::RenderWindow & window) {
  sf::Event event;
  // TODO move this code to input? @MATT S
  //window.setMouseCursorVisible(true);
  while (window.pollEvent(event)) {
    if(event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
      if(buttonBounds.contains(event.mouseButton.x, event.mouseButton.y) == true) {
        start = true;
        std::cout<<"i clicked!"<<std::endl;           
      }
    } else if(event.type == event.MouseMoved) {
      if(buttonBounds.contains(event.mouseMove.x, event.mouseMove.y) == true) {
        button.setColor(sf::Color(250, 20, 20));
      } else {
        button.setColor(sf::Color(150, 50, 50));
      }
    }
  }

}
void lobby::drawLobby(sf::RenderWindow & window) {
  //TODO @Matt S do I want to draw it everytime?
  handleInput(window);
  if (start) welcome.setString("Waiting for other players to start");
  window.draw(welcome);
  window.draw(buttonRect);
  window.draw(button);
}