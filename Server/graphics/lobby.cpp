#include "lobby.h"
#include <iostream>
#include "input.h"

gx::lobby::lobby(void):start(false),ready(false),joined(false){
  font.loadFromFile("arial.ttf");
  welcome.setFont(font);
  welcome.setColor(sf::Color::Red);
  welcome.setCharacterSize(36);
  welcome.setString("Waiting for others to connect.");
  welcome.setPosition(200,200);
  buttonRect.setSize(sf::Vector2f(250,45));
  buttonRect.setOutlineColor(sf::Color::Red);
  buttonRect.setOutlineThickness(2);
  buttonRect.setPosition(280,500);
  button.setFont(font);
  button.setColor(sf::Color(150,50,50));
  button.setCharacterSize(36);
  button.setString("join the game");
  buttonBounds = buttonRect.getGlobalBounds();
  textBounds = button.getGlobalBounds();
  button.setPosition(280+(buttonBounds.width-textBounds.width)/2,500);
  // players
  readyTexture.loadFromFile("graphics/Images/ready.png");
  notReadyTexture.loadFromFile("graphics/Images/notready.png");
}

gx::lobby::~lobby(void) {}

void gx::lobby::handleInput(input const& userInput) {
  if(buttonBounds.contains(userInput.mouseXpos(), userInput.mouseYpos()) == true) {
    button.setColor(sf::Color(250, 20, 20));
    start = userInput.mouseClicked();
  } else {
    button.setColor(sf::Color(150, 50, 50));
  }
}
void gx::lobby::updateLobby(std::vector<std::pair<int,bool>> & playerStatus ) {
  status.clear();
  for (auto itr = playerStatus.begin(); itr != playerStatus.end(); itr++ )
    status.push_back((*itr).second);
  if (!joined) {
    welcome.setString("Please click join to start.");
    joined = true;
    for (int i=0;i<playerStatus.size();i++) {
      players.push_back(sf::Sprite());
    }
  }
  for (int i=0;i<playerStatus.size();i++) {
    if (status[i]) {
      players[i].setTexture(readyTexture);
    } else {
      players[i].setTexture(notReadyTexture);
    }
  }
}


void gx::lobby::drawLobby(sf::RenderWindow & window) {
  if (joined && start) {
    if (!ready) {
      welcome.setString("Waiting for other players to start"); 
      button.setString("cancel");
      buttonBounds = buttonRect.getGlobalBounds();
      textBounds = button.getGlobalBounds();
      button.setPosition(280+(buttonBounds.width-textBounds.width)/2,500);
    } else {
      welcome.setString("Please click join to start.");
      button.setString("join the game");
      buttonBounds = buttonRect.getGlobalBounds();
      textBounds = button.getGlobalBounds();
      button.setPosition(280+(buttonBounds.width-textBounds.width)/2,500);
    }
    ready = !ready;
  } 
  window.draw(welcome);
  window.draw(buttonRect);
  window.draw(button);
  if (joined ) {
    for (int i=0;i<players.size();i++) {
      players[i].setPosition(window.getSize().x/players.size()/2-50+i*(window.getSize().x/players.size()), 300); 
      window.draw(players[i]);
    }
  }
}
