#pragma once
#include <SFML/Graphics.hpp>

class lobby {
  sf::Text  welcome;
  sf::Font  font;
  sf::Text  button;
  sf::FloatRect buttonBounds;
  sf::RectangleShape buttonRect;
  bool start;
public:
  lobby(void);
  ~lobby(void);
  bool getStart() const { return start; }
  void handleInput(sf::RenderWindow & window);
  void drawLobby(sf::RenderWindow & window);
};

