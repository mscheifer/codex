#include "HUD.h"
#include <string>

/* TODO the constants should really be defined somewhere */
gx::HUD::HUD(void):health(100), maxHealth(100), HLossPercentage(0), 
  mana(100), maxMana(100), MLossPercentage(0), canPickUp(false) {
  font.loadFromFile("arial.ttf");
  emptyBarTexture.loadFromFile("graphics/Images/Empty_bar.png");
  //heart image
  heartTexture.loadFromFile("graphics/Images/heart_color.png");
  heartSprite.setTexture(heartTexture);
  heartSprite.setPosition(10,550-7.5);
  //mana image
  manaTexture.loadFromFile("graphics/Images/magic_color.png");
  manaSprite.setTexture(manaTexture);
  manaSprite.setPosition(750,550-7.5);
  //health bar empty 
  hEmptyBarSprite.setTexture(emptyBarTexture);
  hEmptyBarSprite.setPosition(55,550-7.5);
  //heath bar 
  hBarTexture.loadFromFile("graphics/Images/Full_life_bar.png");
  hBarSprite.setTexture(hBarTexture);
  hBarSprite.setPosition(55,550-7.5);
  //heath text
  healthText.setFont(font);
  healthText.setCharacterSize(18);
  healthText.setColor(sf::Color::Black);
  //mana bar empty 
  mEmptyBarSprite.setTexture(emptyBarTexture);
  mEmptyBarSprite.setPosition(545,550-7.5);
  //mana bar  
  mBarTexture.loadFromFile("graphics/Images/Full_mana_bar.png");
  mBarSprite.setTexture(mBarTexture);
  mBarSprite.setPosition(545 ,550-7.5);
  //mana text
  manaText.setFont(font);
  manaText.setCharacterSize(18);
  manaText.setColor(sf::Color::Black);
  //weapin pick up text
  pickUp.setFont(font);
  pickUp.setCharacterSize(24);
  pickUp.setColor(sf::Color::Yellow);
  pickUp.setPosition(100,100);
  //bad guy icon
  badGuyTexture.loadFromFile("graphics/Images/badguy.png");
  badGuySprite.setTexture(badGuyTexture);
  badGuySprite.setPosition(740,10);
  badGuySprite.setScale(0.1,0.1);
  //positionText
  positionText.setFont(font);
  positionText.setCharacterSize(24);
  positionText.setColor(sf::Color::Black);
  positionText.setPosition(10,10);
}

gx::HUD::~HUD(void) {
}

void gx::HUD::draw(sf::RenderWindow & window) {
  //health loss
//  healthLoss.setSize(sf::Vector2f(200*HLossPercentage , 25));
 // healthLoss.setPosition(255-200*HLossPercentage , 550);
  //health display
  std::string healthS(std::to_string(static_cast<long long>(health)) + 
    std::string("/") +std::to_string(static_cast<long long>(maxHealth)));
  healthText.setString(healthS);
  healthRect = manaText.getGlobalBounds();
  healthText.setPosition( 55+ (200 -healthRect.width)/2 , 550);
  //mana display
  std::string manaS(std::to_string(static_cast<long long>(mana)) + 
    std::string("/") +std::to_string(static_cast<long long>(maxMana)));
  manaText.setString(manaS);
  manaRect = manaText.getGlobalBounds();
  manaText.setPosition( 545+(200-manaRect.width)/2 , 550);
  //draw
  window.draw(hEmptyBarSprite);
  window.draw(mEmptyBarSprite);
  window.draw(heartSprite);
  window.draw(manaSprite);
  window.draw(hBarSprite);
  window.draw(mBarSprite);
  window.draw(healthText);
  window.draw(manaText);
  window.draw(positionText);
  if (minotaur) 
    window.draw(badGuySprite);
  if (canPickUp)
    window.draw(pickUp);
}

void gx::HUD::updateHUD(const Player& player) {
  HLossPercentage = (maxHealth-player.getHealth()) / maxHealth;
  MLossPercentage = (maxMana-player.getMana()) / maxMana;
  health = player.getHealth();
  mana = player.getMana();
  minotaur = player.isMinotaur();
  canPickUp = (player.getPickupWeaponType() != UNK); 
  pickUp.setString("Hold F to pick up " + WeaponNames[player.getPickupWeaponType()]);
  hBarSprite.setTextureRect(sf::IntRect(0,0, static_cast<int>(health/maxHealth*200), 40));
  mBarSprite.setTextureRect(sf::IntRect(0,0, static_cast<int>(mana/maxMana*200), 40));
  positionText.setString(std::to_string(static_cast<long double>(player.getPosition().x))+" , "+
    std::to_string(static_cast<long double>(player.getPosition().y))+" , "+
    std::to_string(static_cast<long double>(player.getPosition().z)));  
}
