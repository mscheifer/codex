#include "HUD.h"
#include <string>

/* TODO the constants should really be defined somewhere */
gx::HUD::HUD(void):health(100), maxHealth(100), HLossPercentage(0), 
  mana(100), maxMana(100), MLossPercentage(0), canPickUp(false), currentSelect(0) {
  font.loadFromFile("arial.ttf");
  emptyBarTexture.loadFromFile("graphics/Images/Empty_bar.png");
  //heart image
  heartTexture.loadFromFile("graphics/Images/heart_color.png");
  heartSprite.setTexture(heartTexture);
  heartSprite.setPosition(10,10);
  //mana image
  manaTexture.loadFromFile("graphics/Images/magic_color.png");
  manaSprite.setTexture(manaTexture);
  manaSprite.setPosition(10,60);
  //health bar empty 
  hEmptyBarSprite.setTexture(emptyBarTexture);
  hEmptyBarSprite.setPosition(55,10);
  //heath bar 
  hBarTexture.loadFromFile("graphics/Images/Full_life_bar.png");
  hBarSprite.setTexture(hBarTexture);
  hBarSprite.setPosition(55,10);
  //heath text
  healthText.setFont(font);
  healthText.setCharacterSize(18);
  healthText.setColor(sf::Color::Black);
  //mana bar empty 
  mEmptyBarSprite.setTexture(emptyBarTexture);
  mEmptyBarSprite.setPosition(55,60);
  //mana bar  
  mBarTexture.loadFromFile("graphics/Images/Full_mana_bar.png");
  mBarSprite.setTexture(mBarTexture);
  mBarSprite.setPosition(55,60);
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
  badGuySprite.setPosition(10,110);
  badGuySprite.setScale(0.1f,0.1f);
  //positionText
  positionText.setFont(font);
  positionText.setCharacterSize(24);
  positionText.setColor(sf::Color::Black);
  positionText.setPosition(600,600);
  //aimer
  aimerTexture.loadFromFile("graphics/Images/aimer.png");
  aimer.setTexture(aimerTexture);
  aimer.setPosition(300,200);
  //buffs
  initializeSprites();
}

gx::HUD::~HUD(void) {
  for (auto itr=buffSprites.begin();itr != buffSprites.end(); itr++) {
    delete *itr;
  }
  for (auto itr=buffTextures.begin();itr != buffTextures.end(); itr++) {
    delete *itr;
  }
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
  healthText.setPosition( 55+ (200 -healthRect.width)/2 , 10+7.5);
  //mana display
  //TODO: use stringstrem here
  std::string manaS(std::to_string(static_cast<long long>(mana)) + 
    std::string("/") +std::to_string(static_cast<long long>(maxMana)));
  manaText.setString(manaS);
  manaRect = manaText.getGlobalBounds();
  manaText.setPosition( 55+(200-manaRect.width)/2 , 60+7.5);
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
  if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) { //TODO this is bad
    window.draw(aimer);
  }
  int n = 0; 
  for ( int i =0; i<renderBuff.size(); i++ ) {
    if (renderBuff[i]) {
      buffSprites[i]->setPosition(10+n*37,window.getSize().y-42);
      window.draw(*buffSprites[i]); 
      n++;
    }
  }
  //draw selected weapon
  weaponSprites[currentSelect]->setPosition(window.getSize().x-111, 10);
  window.draw(*weaponSprites[currentSelect]);
  renderWeapon[currentSelect] = false;
  for ( int i =0; i<renderWeapon.size(); i++ ) {
    if (renderWeapon[i]) {
      weaponSprites[i]->setPosition(window.getSize().x-42,26);
      weaponSprites[i]->setScale(0.5,0.5);
      window.draw(*weaponSprites[i]); 
      n = i;
      break;
    }
  }
  weaponSprites[n]->setScale(1,1);
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
  //buffTextures.clear();
  //buffSprites.clear();
  
  positionText.setString(std::to_string(static_cast<long double>(player.getPosition().x))+" , "+
    std::to_string(static_cast<long double>(player.getPosition().y))+" , "+
    std::to_string(static_cast<long double>(player.getPosition().z)));  
 
 //update buffs
  for (auto itr = renderBuff.begin(); itr != renderBuff.end(); itr++ ) {
    *itr = false;
  }
  for (auto itr = player.getBuffs().begin(); itr != player.getBuffs().end(); itr++ ) {
    std::cout<<"i am doing stupid shit with "<<std::endl;
    std::cout<<itr->first<<std::endl;
    std::cout << BuffInfo[(*itr).first].code <<std::endl;
    renderBuff[BuffInfo[(*itr).first].code] =  true; 
  }
 //update weapons
  for (auto itr = renderWeapon.begin(); itr != renderWeapon.end(); itr++ ) {
    *itr = false;
  }
  renderWeapon[player.weapon1] = true;
  renderWeapon[player.weapon2] = true;
  currentSelect = player.getCurrentWeaponSelection();
  std::cout<<"charge time is " << player.elapsedChargeTime <<std::endl;
  std::cout<<"total time is " << player.totalChargeTime <<std::endl;
  std::cout<<"magic type is " << player.chargeMagicType <<std::endl;

}

void gx::HUD::buffHelper(std::string & path) {
   sf::Texture* tText;
   sf::Sprite* tSprite;
   tText = new sf::Texture();
   tSprite = new sf::Sprite();
   tText->loadFromFile(path);
   tSprite->setTexture(*tText);
   tSprite->setScale(0.5,0.5);
   buffTextures.push_back(tText);
   buffSprites.push_back(tSprite);
   renderBuff.push_back(false);
}

void gx::HUD::weaponHelper(std::string & path) {
   sf::Texture* tText;
   sf::Sprite* tSprite;
   tText = new sf::Texture();
   tSprite = new sf::Sprite();
   tText->loadFromFile(path);
   tSprite->setTexture(*tText);
   weaponTextures.push_back(tText);
   weaponSprites.push_back(tSprite);
   renderWeapon.push_back(false);
}

void gx::HUD::initializeSprites() {
   //none
   buffTextures.push_back(new sf::Texture());
   buffSprites.push_back(new sf::Sprite());
   renderBuff.push_back(false);
   buffHelper(std::string("graphics/Images/powerMove.png"));
   buffHelper(std::string("graphics/Images/powerMana.png"));
   buffHelper(std::string("graphics/Images/powerHealth.png"));
   buffHelper(std::string("graphics/Images/powerStr.png"));
   buffHelper(std::string("graphics/Images/powerAttack.png"));
   buffHelper(std::string("graphics/Images/powerCharge.png"));
   buffHelper(std::string("graphics/Images/statF.png"));
   buffHelper(std::string("graphics/Images/statI.png"));
   buffHelper(std::string("graphics/Images/statT.png"));
   buffHelper(std::string("graphics/Images/statG.png"));
   weaponTextures.push_back(new sf::Texture());
   weaponSprites.push_back(new sf::Sprite());
   renderWeapon.push_back(false);
   weaponHelper(std::string("graphics/Images/weaponFire.png"));
   weaponHelper(std::string("graphics/Images/weaponIce.png"));
   weaponHelper(std::string("graphics/Images/weaponThu.png"));
   weaponHelper(std::string("graphics/Images/weaponBasic.png"));
   weaponHelper(std::string("graphics/Images/weaponBasic.png"));
}