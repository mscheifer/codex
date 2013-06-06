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
  int weapon1;
  int weapon2;
  int currentSelect;
  float elapsedChargeTime;
  float totalChargeTime;
  int chargeMagicType;
  bool charging;
  float timer;
  int aimerOuter;
  int aimerInner;
  vector3f playerDirection;
  int hit;
  BUFF ptype;
  vector3f attackedDir;
  float attackedAngle;

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
  sf::Texture energeBarTexture;
  sf::Sprite energeBarSprite;
  sf::Texture energeBarFrameTexture;
  sf::Sprite energeBarFrameSprite;
  std::vector<sf::Texture*> buffTextures;
  std::vector<sf::Sprite*> buffSprites;
  std::vector<sf::Texture*> buffLTextures;
  std::vector<sf::Sprite*> buffLSprites;
  std::vector<sf::Texture*> weaponTextures;
  std::vector<sf::Sprite*> weaponSprites;
  std::vector<sf::Texture*> aimerTextures;
  std::vector<sf::Sprite*> aimerSprites;
  std::vector<sf::Texture*> hitTextures;
  std::vector<sf::Sprite*> hitSprites;
  std::vector<sf::Texture*> hitDirTextures;
  std::vector<sf::Sprite*> hitDirSprites;
  std::vector<bool> renderBuff;
  std::vector<int> remainTime;
  sf::Text currentSpell;
  sf::Text nextSpell;
  sf::Text clockText;
  sf::Clock buffClock;
  sf::Clock hitClock;
  sf::Text collectedPU;
  void buffHelper(std::string & path);
  void buffLHelper(std::string & path);
  void weaponHelper(std::string & path);
  void aimerHelper(std::string & path);
  void hitHelper(std::string & path);
  void hitDirHelper(std::string & path);
  float rotateAngle(vector3f v1, vector3f v2);

public:
  static const int aimerIndex[18][2];
  static const int hitIndex[18];
  HUD(void);
  ~HUD(void);
  void updateHUD(const Player& player);
  void updateDir(vector3f & dir);
  void updateHUDTimer(float timer);
  void draw(sf::RenderWindow & window);
  void initializeSprites();
};
} //end of namespace gx
