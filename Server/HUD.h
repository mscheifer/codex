#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class HUD {
private :
  float health;
  float maxHealth;
  float HLossPercentage;
  /* health max shouldn't be sent over the network
   * we should use the init packet now
   */
  float mana;
  float maxMana;
  float MLossPercentage;
  bool minotaur;
  sf::Text healthText;
  sf::Text manaText;
  sf::Font font;
  sf::RectangleShape healthBar;
  sf::RectangleShape healthLoss;
  sf::RectangleShape manaBar;
  sf::RectangleShape manaLoss; 
  sf::FloatRect healthRect;
  sf::FloatRect manaRect;
  sf::Texture hTexture;
  sf::Sprite hSprite;
  sf::Texture mTexture;
  sf::Sprite mSprite;
  sf::Texture hBarTexture;
  sf::Sprite hBarSprite;
  sf::Texture mBarTexture;
  sf::Sprite mBarSprite;
  sf::Texture badGuyTexture;
  sf::Sprite badGuySprite;
public:
  HUD(void);
  ~HUD(void);
  void updateHUD(const Player& player);
  void draw(sf::RenderWindow & window);
};

