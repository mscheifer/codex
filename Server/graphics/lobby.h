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
  sf::FloatRect textBounds;
  bool start;
  bool ready;
  bool joined;
  std::vector<bool> status;
  sf::Texture readyTexture;
  sf::Texture notReadyTexture;
  std::vector<sf::Sprite> players; 
public:
  lobby(void);
  ~lobby(void);
  bool getStart() const { return start; }
  void endGame() { start = false; }
  void handleInput(input const& userInput);
  void drawLobby(sf::RenderWindow & window);
  void updateLobby(std::vector<std::pair<int,bool>> & playerStatus); 
};

} //end namespace gx
