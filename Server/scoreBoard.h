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
  sf::RectangleShape highlight;
  std::vector<sf::Text> playerScores;
  std::vector<sf::Text> playerKills;
  std::vector<sf::Text> playerWins;
  std::vector<int> pwins;
  std::vector<int> pkills;
  std::vector<bool> pdead;
  unsigned int playerId;
  unsigned int minotaurId;
  sf::Texture badGuyTexture;
  sf::Texture goodGuyTexture;
  std::vector<sf::Sprite> playerSprite;
  int windowSizeX;
  int windowSizeY;
public:
  scoreBoard(int numPlayer);
  ~scoreBoard(void);
  void draw(sf::RenderWindow & window);
  void updateScores(std::vector<int> & pwins, std::vector<int> & pkills, std::vector<bool>& pdead);
  void updateNames(std::vector<std::string> & names);
  void setPlayerId(unsigned int);
  void setMinotaurId(unsigned int);
  void windowResize(int x, int y);
};

