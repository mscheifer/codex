#include "scoreBoard.h"
#include <string>
#include <iostream>

scoreBoard::scoreBoard(int numPlayer) {
 windowSizeX = 0;
 windowSizeY = 0;
 int xOffset = 0;
 int yOffset = 0;
 minotaurId = 0;
 font.loadFromFile("MORPHEUS.TTF");
 boardText.setFont(font);
 boardText.setCharacterSize(24);
 boardText.setColor(sf::Color::White);
 boardText.setString("Score Board");
 highlight.setFillColor(sf::Color(31,69,252,100));
 highlight.setSize(sf::Vector2f(160,25*numPlayer));
 frame.setOutlineColor(sf::Color(100,100,100));
 frame.setOutlineThickness(5);
 frame.setPosition(20+xOffset,20+yOffset);
 frame.setSize(sf::Vector2f(200,25*(numPlayer+3)));
 frame.setFillColor(sf::Color(102,99,98,100));
 sf::FloatRect boardRect = frame.getGlobalBounds(); 
 sf::FloatRect textRect = boardText.getGlobalBounds();
 //TOOD shouldn't hardcode position but might change based on whether using sprites
 boardText.setPosition((boardRect.width-textRect.width)/2+15 + xOffset, 25 + yOffset);
 winText.setFont(font);
 winText.setCharacterSize(18);
 winText.setColor(sf::Color::White);
 winText.setString("Wins");
 winText.setPosition(155 + xOffset, 55 + yOffset);
 killText.setFont(font);
 killText.setCharacterSize(18);
 killText.setColor(sf::Color::White);
 killText.setString("Kills");
 killText.setPosition(110+xOffset, 55+yOffset);

 badGuyTexture.loadFromFile("graphics/Images/BG_Icon.png");
 goodGuyTexture.loadFromFile("graphics/Images/GG_Icon.png");

 for (int i = 0; i<numPlayer; i++ ) {
   playerSprite.push_back(sf::Sprite());
   playerSprite[i].setTexture(goodGuyTexture);
   playerSprite[i].setPosition(25+xOffset, 25*(i+3)+5+yOffset);
   playerSprite[i].setScale(0.16f,0.16f);
   playerScores.push_back(sf::Text());
   playerScores[i].setFont(font);
   playerScores[i].setCharacterSize(18);
   playerScores[i].setColor(sf::Color::White);
   playerScores[i].setPosition(55+xOffset, 25*(i+3)+5+yOffset);
   playerKills.push_back(sf::Text());
   playerKills[i].setFont(font);
   playerKills[i].setCharacterSize(18);
   playerKills[i].setColor(sf::Color::White);
   playerKills[i].setString("0");
   playerKills[i].setPosition(110+xOffset, 25*(i+3)+5+yOffset);
   playerWins.push_back(sf::Text());
   playerWins[i].setFont(font);
   playerWins[i].setCharacterSize(18);
   playerWins[i].setColor(sf::Color::White);
   playerWins[i].setString("0");
   playerWins[i].setPosition(155+xOffset, 25*(i+3)+5+yOffset);
   pkills.push_back(0);
   pwins.push_back(0);
   pdead.push_back(false);
 }
}


scoreBoard::~scoreBoard(void) {
}

void scoreBoard::draw(sf::RenderWindow & window) {
  windowResize(window.getSize().x, window.getSize().y);
  window.draw(frame);
  window.draw(boardText);
  window.draw(killText);
  window.draw(winText);
  window.draw(highlight);
  for (unsigned int i=0; i<playerWins.size(); i++ ) {
    playerKills[i].setString(std::to_string((long long)pkills[i]));
    playerWins[i].setString(std::to_string((long long) pwins[i]));
    if(pdead[i])
    {
      playerKills[i].setColor(sf::Color(182, 182, 180));
      playerWins[i].setColor(sf::Color(182, 182, 180));
      playerScores[i].setColor(sf::Color(182, 182, 180));
    }
    else 
    {
      playerKills[i].setColor(sf::Color::White);
      playerWins[i].setColor(sf::Color::White);
      playerScores[i].setColor(sf::Color::White);
    }
    window.draw(playerScores[i]);
    window.draw(playerKills[i]);
    window.draw(playerWins[i]);
    window.draw(playerSprite[i]);
  }
}

void scoreBoard::windowResize(int x, int y)
{
  if (windowSizeX == x && windowSizeY == y)
    return;
  windowSizeX = x;
  windowSizeY = y;
  frame.setSize(sf::Vector2f(x*.6, y*.6));
  frame.setPosition(.5*x - frame.getSize().x/2, .5*y - frame.getSize().y/2);
  sf::FloatRect boardRect = frame.getGlobalBounds(); 
  sf::FloatRect textRect = boardText.getGlobalBounds();
  int xOffset = x/2 - boardRect.width/2;
  int yOffset = y/2 - boardRect.height/2;
  highlight.setSize(sf::Vector2f(boardRect.width-80,25));
  
  

  boardText.setPosition((boardRect.width-textRect.width)/2+15 + xOffset, 25 + yOffset);
  boardText.setPosition((boardRect.width-textRect.width)/2+15 + xOffset, 25 + yOffset);
  winText.setPosition(x/2 + x*.1, 25 + boardText.getPosition().y);
  killText.setPosition(x/2 + x*.2, 25 + boardText.getPosition().y);
  for (int i = 0; i<playerWins.size(); i++ ) {
   playerSprite.push_back(sf::Sprite());
   playerSprite[i].setPosition(25+xOffset, 25*(i+3)+5+yOffset);
   playerScores[i].setPosition(55+xOffset, 25*(i+3)+5+yOffset);
   playerKills[i].setPosition(killText.getPosition().x, 25*(i+3)+5+yOffset);
   playerWins[i].setPosition(winText.getPosition().x, 25*(i+3)+5+yOffset);
 }
  setPlayerId(playerId);
}

void scoreBoard::updateScores(std::vector<int> & pwins, std::vector<int> & pkills, std::vector<bool>& pdead){
  this->pwins = pwins;
  this->pkills = pkills;
  this->pdead = pdead;
}
void scoreBoard::updateNames(std::vector<std::string> & names) {
  for (int i=0;i<playerScores.size();i++ ) {
    playerScores[i].setString(names[i]);
  }
}

void scoreBoard::setPlayerId(unsigned int x)
{
  playerId = x;
  this->highlight.setPosition(playerScores[x].getPosition());
  std::cout<<"Changing player's highlight position to "<<this->highlight.getPosition().x <<"," << this->highlight.getPosition().y<<std::endl;
}

void scoreBoard::setMinotaurId(unsigned int id)
{
  if(id == minotaurId || id >= playerScores.size())
    return;
  playerSprite[minotaurId].setTexture(goodGuyTexture);
  minotaurId = id;
  playerSprite[minotaurId].setTexture(badGuyTexture);
}