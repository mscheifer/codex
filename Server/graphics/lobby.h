#pragma once
#include <SFML/Graphics.hpp>

namespace gx {
class input;

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
  void endGame() { start = false; }
  void handleInput(input const& userInput);
  void drawLobby(sf::RenderWindow & window);
};

} //end namespace gx
