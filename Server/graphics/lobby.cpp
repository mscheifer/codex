#include "lobby.h"
#include <iostream>
#include "input.h"

gx::lobby::lobby(void):start(false),ready(false),connected(false),clickedIP(false){
  font.loadFromFile("MORPHEUS.TTF");
  welcome.setFont(font);
  welcome.setColor(sf::Color::White);
  welcome.setCharacterSize(36);
  welcome.setString("Please connect to the server");
  welcome.setPosition(200,200);
  buttonRect.setSize(sf::Vector2f(250,45));
  buttonRect.setOutlineColor(sf::Color::Black);
  buttonRect.setOutlineThickness(2);
  buttonRect.setPosition(280,500);
  button.setFont(font);
  button.setColor(sf::Color(150,50,50));
  button.setCharacterSize(36);
  button.setString("connect");
  buttonBounds = buttonRect.getGlobalBounds();
  textBounds = button.getGlobalBounds();
  button.setPosition(280+(buttonBounds.width-textBounds.width)/2,500);
  // players
  readyTexture.loadFromFile("graphics/Images/ready.png");
  notReadyTexture.loadFromFile("graphics/Images/notready.png");
  IPText.setFont(font);
  IPText.setColor(sf::Color::Black);
  IPText.setCharacterSize(24);
  IPText.setPosition(290,407);
  IPBox.setSize(sf::Vector2f(250,45));
  IPBox.setOutlineColor(sf::Color::Black);
  IPBox.setOutlineThickness(2);
  IPBox.setPosition(280,400);
  backGroundTexture.loadFromFile("graphics/Images/splashBG.png");
  backGroundSprite.setTexture(backGroundTexture);
  backGroundSprite.setPosition(0,0);
  codexTexture.loadFromFile("graphics/Images/splashText.png");
  codexSprite.setTexture(codexTexture);
  codexSprite.setPosition(0,0);
  circleTexture.loadFromFile("graphics/Images/splashCircles.png");
  circleSprite.setTexture(circleTexture);
  circleSprite.setOrigin(circleTexture.getSize().x/2,circleTexture.getSize().y/2);
  timer.restart();
  instructionTexture.loadFromFile("graphics/Images/instructions.png");
  instructionSprite.setTexture(instructionTexture);
}

gx::lobby::~lobby(void) {}

void gx::lobby::handleInput(input & userInput) {
  //clicked the button
  if(buttonBounds.contains(userInput.mouseXpos(), userInput.mouseYpos()) == true) {
    button.setColor(sf::Color::Red);
    start = userInput.mouseClicked();
    if (connected) {
      if (start) {
        if (!ready && inputText.compare("")) {
          welcome.setString("Waiting for other players to start"); 
          button.setString("Cancel");
          buttonBounds = buttonRect.getGlobalBounds();
          textBounds = button.getGlobalBounds();
          button.setPosition(280+(buttonBounds.width-textBounds.width)/2,500);
          ready = true;
        } else {
          welcome.setString("Please enter a name and start.");
          button.setString("Start");
          buttonBounds = buttonRect.getGlobalBounds();
          textBounds = button.getGlobalBounds();
          button.setPosition(280+(buttonBounds.width-textBounds.width)/2,500);
          ready = false;
        }
      }
    } 
  } else {
    button.setColor(sf::Color::Black);
    start = false;
  }
  if (userInput.mouseClicked()){
    clickedIP = IPBox.getGlobalBounds().contains(userInput.mouseXpos(), userInput.mouseYpos());
    userInput.setTextMode(clickedIP);
  } 
  if (userInput.getTextMode())
    IPBox.setOutlineColor(sf::Color::Red);
  else     
    IPBox.setOutlineColor(sf::Color::Black);
  if (userInput.getTextMode()) this->inputText = userInput.getInputText();
  //std::cout<<"I am printing" <<userInput.getInputText()<<std::endl;
}
void gx::lobby::updateLobby(std::vector<std::pair<int,bool>> & playerStatus ) {
  status.clear();
  for (auto itr = playerStatus.begin(); itr != playerStatus.end(); itr++ )
    status.push_back((*itr).second);

  //players.clear();
  int size = players.size();
  for (int i=0; i<playerStatus.size()-size;i++ ) {
    players.push_back(sf::Sprite());
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
  backGroundSprite.setScale(static_cast<float>(window.getSize().x)/backGroundTexture.getSize().x,
  static_cast<float>(window.getSize().y)/backGroundTexture.getSize().y);
  circleSprite.setRotation((static_cast<int>(this->timer.getElapsedTime().asMilliseconds())% 25000000 ) *72/5000); 
  instructionSprite.setPosition(window.getSize().x-instructionTexture.getSize().x ,0);
  window.draw(backGroundSprite);
  window.draw(instructionSprite);
  window.draw(circleSprite);
  window.draw(codexSprite);
 // std::cout<<"connected is "<< connected<<" start is "<< start<< " ready is "<< ready <<std::endl;
  window.draw(welcome);
  window.draw(buttonRect);
  window.draw(button);
  window.draw(IPBox);
  IPText.setString(inputText);
  window.draw(IPText);
  if (connected ) {
    for (int i=0;i<players.size();i++) {
      players[i].setPosition(window.getSize().x/players.size()/2-50+i*(window.getSize().x/players.size()), 300); 
      window.draw(players[i]);
    }
  }
//  std::cout<<"window size is"<<window.getSize().x<<" , "<<window.getSize().y<<std::endl;
 // std::cout<<"                                        image size is"<<backGroundTexture.getSize().x<<" , "<<backGroundTexture.getSize().y<<std::endl;
}

void gx::lobby::setConnected(bool connected) {
  this->connected = connected;
  if (!connected) {
    welcome.setString("Wrong IP address");
  } else { 
    inputText = ""; 
    welcome.setString("Please enter a name and start.");
    button.setString("Start");
  }
}