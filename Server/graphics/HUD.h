#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <vector>

namespace gx {
  
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
  bool canPickUp;
  sf::Text healthText;
  sf::Text manaText;
  sf::Font font;
  sf::Text positionText;
  //sf::RectangleShape healthBar;
  //sf::RectangleShape healthLoss;
  sf::FloatRect healthRect;
  sf::FloatRect manaRect;
  sf::Texture heartTexture;
  sf::Sprite heartSprite;
  sf::Texture manaTexture;
  sf::Sprite manaSprite;
  sf::Texture emptyBarTexture;
  sf::Sprite hEmptyBarSprite;
  sf::Sprite mEmptyBarSprite;
  sf::Texture hBarTexture;
  sf::Sprite hBarSprite;
  sf::Texture mBarTexture;
  sf::Sprite mBarSprite;
  sf::Texture badGuyTexture;
  sf::Sprite badGuySprite;
  sf::Text pickUp;
  sf::Texture aimerTexture;
  sf::Sprite aimer;
  std::vector<sf::Texture*> buffTextures;
  std::vector<sf::Sprite*> buffSprites;
  std::vector<sf::Texture*> weaponTextures;
  std::vector<sf::Sprite*> weaponSprites;
  std::vector<bool> renderBuff;
  std::vector<bool> renderWeapon;
  int currentSelect;
  void buffHelper(std::string & path);
  void weaponHelper(std::string & path);

public:
  HUD(void);
  ~HUD(void);
  void updateHUD(const Player& player);
  void draw(sf::RenderWindow & window);
  void initializeSprites();
};
} //end of namespace gx
