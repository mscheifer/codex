#include "scoreBoard.h"
#include <string>
#include <iostream>

scoreBoard::scoreBoard(int numPlayer) {
 minotaurId = 9999;
 font.loadFromFile("MORPHEUS.TTF");
 boardText.setFont(font);
 boardText.setCharacterSize(24);
 boardText.setColor(sf::Color::Black);
 boardText.setString("Score Board");
 highlight.setFillColor(sf::Color(31,69,252,100));
 highlight.setSize(sf::Vector2f(160,25*numPlayer));
 frame.setOutlineColor(sf::Color(100,100,100));
 frame.setOutlineThickness(5);
 frame.setPosition(20,20);
 frame.setSize(sf::Vector2f(200,25*(numPlayer+3)));
 frame.setFillColor(sf::Color(102,99,98,100));
 sf::FloatRect boardRect = frame.getGlobalBounds(); 
 sf::FloatRect textRect = boardText.getGlobalBounds();
 //TOOD shouldn't hardcode position but might change based on whether using sprites
 boardText.setPosition((boardRect.width-textRect.width)/2+15, 25);
 winText.setFont(font);
 winText.setCharacterSize(18);
 winText.setColor(sf::Color::Black);
 winText.setString("Wins");
 winText.setPosition(155, 55);
 killText.setFont(font);
 killText.setCharacterSize(18);
 killText.setColor(sf::Color::Black);
 killText.setString("Kills");
 killText.setPosition(110, 55);

 badGuyTexture.loadFromFile("graphics/Images/badguy.png");
 goodGuyTexture.loadFromFile("graphics/Images/badguy.png");

 for (int i = 0; i<numPlayer; i++ ) {
   playerSprite.push_back(sf::Sprite());
   playerSprite[i].setTexture(goodGuyTexture);
   playerSprite[i].setPosition(25, 25*(i+3)+5);
   playerSprite[i].setScale(0.06f,0.06f);
   playerScores.push_back(sf::Text());
   playerScores[i].setFont(font);
   playerScores[i].setCharacterSize(18);
   playerScores[i].setColor(sf::Color::Black);
   playerScores[i].setPosition(55, 25*(i+3)+5);
   playerKills.push_back(sf::Text());
   playerKills[i].setFont(font);
   playerKills[i].setCharacterSize(18);
   playerKills[i].setColor(sf::Color::Black);
   playerKills[i].setString("0");
   playerKills[i].setPosition(110, 25*(i+3)+5);
   playerWins.push_back(sf::Text());
   playerWins[i].setFont(font);
   playerWins[i].setCharacterSize(18);
   playerWins[i].setColor(sf::Color::Black);
   playerWins[i].setString("0");
   playerWins[i].setPosition(155, 25*(i+3)+5);
   pkills.push_back(0);
   pwins.push_back(0);
   pdead.push_back(false);
 }

}


scoreBoard::~scoreBoard(void) {
}

void scoreBoard::draw(sf::RenderWindow & window) {
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
      playerKills[i].setColor(sf::Color::Black);
      playerWins[i].setColor(sf::Color::Black);
      playerScores[i].setColor(sf::Color::Black);
    }
    window.draw(playerScores[i]);
    window.draw(playerKills[i]);
    window.draw(playerWins[i]);
    window.draw(playerSprite[i]);
  }
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
  //if(id == minotaurId)
  //  return;
  //playerSprite[minotaurId].setTexture(goodGuyTexture);
  //minotaurId = id;
  //playerSprite[minotaurId].setTexture(badGuyTexture);
}