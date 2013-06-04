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
  bool start; //clicked ready button
  bool ready; //display ready
  bool connected;
  bool clickedIP;//for IP entering
  std::vector<bool> status;
  sf::Texture readyTexture;
  sf::Texture notReadyTexture;
  std::vector<sf::Sprite> players; 
  sf::Text IPText;
  sf::RectangleShape IPBox;
  std::string inputText;
  sf::Texture backGroundTexture;
  sf::Sprite backGroundSprite;
public:
  lobby(void);
  ~lobby(void);
  bool getStart() const { return start; }
  std::string getInputText() const { return inputText;}
  void endGame() { start = false; }
  void handleInput(input & userInput);
  void drawLobby(sf::RenderWindow & window);
  void updateLobby(std::vector<std::pair<int,bool>> & playerStatus); 
  void setConnected(bool connected);
};

} //end namespace gx
