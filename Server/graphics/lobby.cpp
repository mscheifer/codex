#include "lobby.h"
#include <iostream>
#include "input.h"

gx::lobby::lobby(void):start(false)  {
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

gx::lobby::~lobby(void) {}

void gx::lobby::handleInput(input const& userInput) {
  if(buttonBounds.contains(userInput.mouseXpos(), userInput.mouseYpos()) == true) {
    button.setColor(sf::Color(250, 20, 20));
    if(userInput.fire1()) {
      start = true;
    }
  } else {
    button.setColor(sf::Color(150, 50, 50));
  }
}

void gx::lobby::drawLobby(sf::RenderWindow & window) {
  if (start) welcome.setString("Waiting for other players to start");
  else welcome.setString("Please click join to start.");
  window.draw(welcome);
  window.draw(buttonRect);
  window.draw(button);
}
