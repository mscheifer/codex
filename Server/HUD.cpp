#include "HUD.h"
#include <string>

/* TODO the constants should really be defined somewhere */
HUD::HUD(void):health(100), maxHealth(100), HLossPercentage(0), 
  mana(100), maxMana(100), MLossPercentage(0) {
  font.loadFromFile("arial.ttf");
  healthBar.setSize(sf::Vector2f(200,25));
  //healthBar.setOutlineColor(sf::Color::Black);
  //healthBar.setOutlineThickness(2);
  healthBar.setFillColor(sf::Color::Green);
  healthBar.setPosition(10,550);
  healthLoss.setFillColor(sf::Color::Red);
  //healthLoss.setOutlineColor(sf::Color::Black);
  //healthLoss.setOutlineThickness(2)
  healthText.setFont(font);
  healthText.setCharacterSize(24);
  healthText.setColor(sf::Color::Black);
  manaBar.setSize(sf::Vector2f(200,25));
  //manaBar.setOutlineColor(sf::Color::Black);
  //manaBar.setOutlineThickness(2);
  manaBar.setFillColor(sf::Color::Color(100,149,237));
  manaBar.setPosition(590 ,550);
  //manaLoss.setOutlineColor(sf::Color::Black);
  //manaLoss.setOutlineThickness(2);
  manaLoss.setFillColor(sf::Color::Blue);
  manaText.setFont(font);
  manaText.setCharacterSize(24);
  manaText.setColor(sf::Color::Black);
}

HUD::~HUD(void) {
}

void HUD::draw(sf::RenderWindow & window) {
  //health loss
  healthLoss.setSize(sf::Vector2f(200*HLossPercentage , 25));
  healthLoss.setPosition(210-200*HLossPercentage , 550);
  //health display
  std::string healthS(std::to_string((long long) health) + 
    std::string("/") +std::to_string((long long) maxHealth));
  healthText.setString(healthS);
  healthRect = manaText.getGlobalBounds();
  healthText.setPosition( 10+ (100 -healthRect.width)/2 , 573);
  //mana loss
  manaLoss.setSize(sf::Vector2f(200*MLossPercentage,25));
  manaLoss.setPosition(790-200*MLossPercentage,550);
  //mana display
  std::string manaS(std::to_string((long long) mana) + 
    std::string("/") +std::to_string((long long) maxMana));
  manaText.setString(manaS);
  manaRect = manaText.getGlobalBounds();
  manaText.setPosition( 590+(200-manaRect.width)/2 , 573);
  //draw
  window.draw(healthBar);
  window.draw(healthLoss);
  window.draw(healthText);
  window.draw(manaBar);
  window.draw(manaLoss);
  window.draw(manaText);
}

void HUD::updateHUD(Player & player) {
  HLossPercentage = (maxHealth-player.getHealth()) / maxHealth;
  MLossPercentage = (maxMana-player.getMana()) / maxMana;
  health = player.getHealth();
  mana = player.getMana();
}