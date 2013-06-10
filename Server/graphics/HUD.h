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
  bool switched;
  std::vector<vector3f> playerPositions;
  float miniMapProx;
  bool doMiniMap;
  int energeBarIndex;

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
  sf::Texture goodGuyTexture;
  sf::Sprite goodGuySprite;
  sf::Sprite winnerSprite;
  sf::Text pickUp;
  sf::Texture energeBarFrameTexture;
  sf::Sprite energeBarFrameSprite;
  sf::Texture miniMapTexture;
  sf::Sprite miniMapSprite;
  sf::Texture miniMapBlipTexture;
  sf::Texture minoCorruptHUDTexture1;
  sf::Sprite minoCorruptHUDSprite1;
  sf::Texture minoCorruptHUDTexture2;
  sf::Sprite minoCorruptHUDSprite2;
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
  std::vector<sf::Sprite*> playerSprites; //for minimap
  std::vector<sf::Texture*> energeBarTextures;
  std::vector<sf::Sprite*> energeBarSprites;
  std::vector<bool> renderBuff;
  std::vector<int> remainTime;
  sf::Text currentSpell;
  sf::Text nextSpell;
  sf::Text clockText;
  sf::Clock buffClock;
  sf::Clock hitClock;
  sf::Clock minoHUDClock;
  sf::Text collectedPU;
  sf::Texture deathTexture;
  sf::Sprite deathScreen;
  void buffHelper(std::string & path);
  void buffLHelper(std::string & path);
  void weaponHelper(std::string & path);
  void aimerHelper(std::string & path);
  void hitHelper(std::string & path);
  void hitDirHelper(std::string & path);
  void energeBarHelper(std::string & path);
  float rotateAngle(vector3f v1, vector3f v2);
  float rotateAngleRad(vector3f v1, vector3f v2);

public:
  static const int aimerIndex[19][3];
  static const int hitIndex[19];
  HUD(void);
  ~HUD(void);
  void updateHUD(int id, const std::vector<Player>& players);
  void updateDir(vector3f & dir);
  void updateHUDTimer(float timer);
  void draw(sf::RenderWindow & window);
  void initializeSprites();
  void setWinner(Game_State w);
};
} //end of namespace gx
