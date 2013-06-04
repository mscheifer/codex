#pragma once
#include <SFML/Graphics.hpp>
#include <vector> 

class scoreBoard {
  sf::Text boardText;
  sf::Text killText;
  sf::Text winText;
  sf::Font font;
  sf::RectangleShape frame;
  sf::RectangleShape board;
  std::vector<sf::Text> playerScores;
  std::vector<sf::Text> playerKills;
  std::vector<sf::Text> playerWins;
  std::vector<int> pwins;
  std::vector<int> pkills;
public:
  scoreBoard(int numPlayer);
  ~scoreBoard(void);
  void draw(sf::RenderWindow & window);
  void updateScores(std::vector<int> & pwins, std::vector<int> & pkills);
  void updateNames(std::vector<std::string> & names);
};

