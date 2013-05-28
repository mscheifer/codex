#include "HUD.h"
#include <string>

/* TODO the constants should really be defined somewhere */
HUD::HUD(void):health(100), maxHealth(100), HLossPercentage(0), 
  mana(100), maxMana(100), MLossPercentage(0) {
  /* test sprite */
  hTexture.loadFromFile("graphics/Images/heart_color.png");
  hSprite.setTexture(hTexture);
  hSprite.setPosition(10,550-7.5);
  hSprite.setScale(0.15,0.15);
  hBarTexture.loadFromFile("graphics/Images/bar.png");
  hBarSprite.setTexture(hBarTexture);
  hBarSprite.setPosition(50,550-7.5);
  hBarSprite.setScale(0.1,0.15);
  mTexture.loadFromFile("graphics/Images/magic_color.png");
  mSprite.setTexture(mTexture);
  mSprite.setPosition(750,550-7.5);
  mSprite.setScale(0.15,0.15);
  mBarTexture.loadFromFile("graphics/Images/bar.png");
  mBarSprite.setTexture(mBarTexture);
  mBarSprite.setPosition(537,550-7.5);
  mBarSprite.setScale(0.1,0.15);
  badGuyTexture.loadFromFile("graphics/Images/badguy.png");
  badGuySprite.setTexture(badGuyTexture);
  badGuySprite.setPosition(740,10);
  badGuySprite.setScale(0.1,0.1);
  /* end */
  font.loadFromFile("arial.ttf");
  healthBar.setSize(sf::Vector2f(200,25));
  //healthBar.setOutlineColor(sf::Color::Black);
  //healthBar.setOutlineThickness(2);
  healthBar.setFillColor(sf::Color::Color(103,20,20));
  healthBar.setPosition(55,550);
  healthLoss.setFillColor(sf::Color::Color(100,100,100));
  //healthLoss.setOutlineColor(sf::Color::Black);
  //healthLoss.setOutlineThickness(2)
  healthText.setFont(font);
  healthText.setCharacterSize(24);
  healthText.setColor(sf::Color::Black);
  manaBar.setSize(sf::Vector2f(200,25));
  //manaBar.setOutlineColor(sf::Color::Black);
  //manaBar.setOutlineThickness(2);
  manaBar.setFillColor(sf::Color(30,103,156));
  manaBar.setPosition(545 ,550);
  //manaLoss.setOutlineColor(sf::Color::Black);
  //manaLoss.setOutlineThickness(2);
  manaLoss.setFillColor(sf::Color::Color(100,100,100));
  manaText.setFont(font);
  manaText.setCharacterSize(24);
  manaText.setColor(sf::Color::Black);
}

HUD::~HUD(void) {
}

void HUD::draw(sf::RenderWindow & window) {
  //health loss
  healthLoss.setSize(sf::Vector2f(200*HLossPercentage , 25));
  healthLoss.setPosition(255-200*HLossPercentage , 550);
  //health display
  std::string healthS(std::to_string((long long) health) + 
    std::string("/") +std::to_string((long long) maxHealth));
  healthText.setString(healthS);
  healthRect = manaText.getGlobalBounds();
  healthText.setPosition( 55+ (200 -healthRect.width)/2 , 573);
  //mana loss
  manaLoss.setSize(sf::Vector2f(200*MLossPercentage,25));
  manaLoss.setPosition(745-200*MLossPercentage,550);
  //mana display
  std::string manaS(std::to_string((long long) mana) + 
    std::string("/") +std::to_string((long long) maxMana));
  manaText.setString(manaS);
  manaRect = manaText.getGlobalBounds();
  manaText.setPosition( 545+(200-manaRect.width)/2 , 573);
  //draw
  window.draw(healthBar);
  window.draw(healthLoss);
  window.draw(healthText);
  window.draw(manaBar);
  window.draw(manaLoss);
  window.draw(manaText);
  window.draw(hSprite);
  window.draw(mSprite);
  window.draw(hBarSprite);
  window.draw(mBarSprite);
  if (minotaur) 
    window.draw(badGuySprite);
}

void HUD::updateHUD(const Player& player) {
  HLossPercentage = (maxHealth-player.getHealth()) / maxHealth;
  MLossPercentage = (maxMana-player.getMana()) / maxMana;
  health = player.getHealth();
  mana = player.getMana();
  minotaur = player.isMinotaur();
}
