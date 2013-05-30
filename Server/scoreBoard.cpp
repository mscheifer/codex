#include "scoreBoard.h"
#include <string>
#include <iostream>

scoreBoard::scoreBoard(int numPlayer) {
 font.loadFromFile("arial.ttf");
 boardText.setFont(font);
 boardText.setCharacterSize(24);
 boardText.setColor(sf::Color::Black);
 boardText.setString("Score Board");
 frame.setOutlineColor(sf::Color(100,100,100));
 frame.setOutlineThickness(5);
 frame.setPosition(20,20);
 frame.setSize(sf::Vector2f(200,25*(numPlayer+3)));
 frame.setFillColor(sf::Color(255,255,0,100));
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
 for (int i = 0; i<numPlayer; i++ ) {
   playerScores.push_back(sf::Text());
   playerScores[i].setFont(font);
   playerScores[i].setCharacterSize(18);
   playerScores[i].setColor(sf::Color::Black);
   playerScores[i].setString(std::string("Player ") +std::to_string((long long)i) );
   playerScores[i].setPosition(25, 25*(i+3)+5);
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
 }

}


scoreBoard::~scoreBoard(void) {
}

void scoreBoard::draw(sf::RenderWindow & window) {
  window.draw(frame);
  window.draw(boardText);
  window.draw(killText);
  window.draw(winText);
  for (unsigned int i=0; i<playerWins.size(); i++ ) {
    playerKills[i].setString(std::to_string((long long)pkills[i]));
    playerWins[i].setString(std::to_string((long long) pwins[i]));
    window.draw(playerScores[i]);
    window.draw(playerKills[i]);
    window.draw(playerWins[i]);
  }
}

void scoreBoard::updateScores(std::vector<int> & pwins, std::vector<int> & pkills){
  this->pwins = pwins;
  this->pkills = pkills;
}
