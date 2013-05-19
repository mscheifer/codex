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
  sf::Text healthText;
  sf::Text manaText;
  sf::Font font;
  sf::RectangleShape healthBar;
  sf::RectangleShape healthLoss;
  sf::RectangleShape manaBar;
  sf::RectangleShape manaLoss; 
  sf::FloatRect healthRect;
  sf::FloatRect manaRect;
public:
  HUD(void);
  ~HUD(void);
  void updateHUD(Player & player);
  void draw(sf::RenderWindow & window);
};

