#include "HUD.h"
#include <string>
#include "Projectile.h"

/* TODO the constants should really be defined somewhere */
gx::HUD::HUD(void):health(100), maxHealth(100), HLossPercentage(0), 
  mana(100), maxMana(100), MLossPercentage(0), canPickUp(false),
  weapon1(0), weapon2(0), currentSelect(0), elapsedChargeTime(0),
  totalChargeTime(-1), chargeMagicType(0), charging(false), timer(0){
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
  positionText.setColor(sf::Color::Green);
  positionText.setPosition(300,500);
  //aimer
  aimerTexture.loadFromFile("graphics/Images/aimer.png");
  aimer.setTexture(aimerTexture);
  //buffs
  initializeSprites();
  //chargig
  energeBarFrameTexture.loadFromFile("graphics/Images/chargebarempty.png"); 
  energeBarFrameSprite.setTexture(energeBarFrameTexture);
  energeBarTexture.loadFromFile("graphics/Images/chargebar.png"); 
  energeBarSprite.setTexture(energeBarTexture);
  //spells
  currentSpell.setFont(font);
  nextSpell.setFont(font);
  currentSpell.setCharacterSize(18);
  nextSpell.setCharacterSize(18);
  currentSpell.setColor(sf::Color::Black);
  nextSpell.setColor(sf::Color::Black);
  clockText.setFont(font);
  clockText.setCharacterSize(36);
  clockText.setColor(sf::Color::Yellow);
}

gx::HUD::~HUD(void) {
  for (auto itr=buffSprites.begin();itr != buffSprites.end(); itr++) {
    delete *itr;
  }
  for (auto itr=buffTextures.begin();itr != buffTextures.end(); itr++) {
    delete *itr;
  }
  for (auto itr=buffLSprites.begin();itr != buffLSprites.end(); itr++) {
    delete *itr;
  }
  for (auto itr=buffLTextures.begin();itr != buffLTextures.end(); itr++) {
    delete *itr;
  }
  for (auto itr=weaponSprites.begin();itr != weaponSprites.end(); itr++) {
    delete *itr;
  }
  for (auto itr=weaponTextures.begin();itr != weaponTextures.end(); itr++) {
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
  aimer.setPosition((window.getSize().x-200)/2, (window.getSize().y-200)/2);
  if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && charging) { //TODO this is bad
    window.draw(aimer);
  }
  int buffn = 0; 
  for ( int i =0; i<renderBuff.size(); i++ ) {
    if (renderBuff[i]) {
      buffSprites[i]->setPosition(10+buffn*37,window.getSize().y-42);
      buffLSprites[i]->setPosition(10+buffn*37,window.getSize().y-42);
      if (remainTime[i]<1000 && (remainTime[i]/100 % 2))
        window.draw(*buffLSprites[i]);
      else 
        window.draw(*buffSprites[i]);
      buffn++;
    }
  }
  //draw selected weapon
  int index1 = (currentSelect) ? weapon2 : weapon1;
  int index2 = (currentSelect) ? weapon1 : weapon2;
  weaponSprites[index1]->setPosition(window.getSize().x-111, 10);
  window.draw(*weaponSprites[index1]);
  weaponSprites[index2]->setPosition(window.getSize().x-42,26);
  weaponSprites[index2]->setScale(0.5,0.5);
  window.draw(*weaponSprites[index2]); 
  weaponSprites[index2]->setScale(1,1);

  //charge bar
  if (totalChargeTime != -1) {
    energeBarFrameSprite.setPosition((window.getSize().x-400)/2,window.getSize().y*0.8);
    energeBarSprite.setPosition((window.getSize().x-400)/2,window.getSize().y*(0.8));
    window.draw(energeBarFrameSprite);
    window.draw(energeBarSprite);
    nextSpell.setPosition( (window.getSize().x-400)/2+400-nextSpell.getGlobalBounds().width,  window.getSize().y*0.8+20 );
    window.draw(nextSpell);
  }
  if (charging){ 
    currentSpell.setPosition( (window.getSize().x-400)/2,  window.getSize().y*0.8+20 );
    window.draw(currentSpell);
  } 
  std::cout<<"time left is "<<timer<<std::endl;
  if (timer > 0 ) {
    clockText.setString(std::string("Game starts in ") +
      std::to_string(static_cast<long long>(timer)) +
      std::string(" seconds"));
    clockText.setPosition((window.getSize().x -clockText.getGlobalBounds().width)/2,200);
    window.draw(clockText);
  }
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
  if (totalChargeTime)
    energeBarSprite.setTextureRect(sf::IntRect(0,0,static_cast<int>(elapsedChargeTime/totalChargeTime*400),15));  

  positionText.setString(std::to_string(static_cast<long double>(player.getPosition().x))+" , "+
    std::to_string(static_cast<long double>(player.getPosition().y))+" , "+
    std::to_string(static_cast<long double>(player.getPosition().z)));  
 
 //update buffs
  for (auto itr = renderBuff.begin(); itr != renderBuff.end(); itr++ ) {
    *itr = false;
  }
  for (auto itr = player.getBuffs().begin(); itr != player.getBuffs().end(); itr++ ) {
    renderBuff[BuffInfo[(*itr).first].code] =  true; 
    remainTime[BuffInfo[(*itr).first].code] = itr->second;
  }
 //update weapons
  weapon1 = player.weapon1;
  weapon2 = player.weapon2;
  currentSelect = player.getCurrentWeaponSelection();
  this->elapsedChargeTime = player.elapsedChargeTime;
  this->totalChargeTime = player.totalChargeTime;
  this->chargeMagicType = player.chargeMagicType;
  //std::cout<<"charge time is " << player.elapsedChargeTime <<std::endl;
  //std::cout<<"total time is " << player.totalChargeTime <<std::endl;
  //std::cout<<"magic type is " << player.chargeMagicType <<std::endl;
  charging = player.charging;
  //spell
  if (chargeMagicType>=0) {
    currentSpell.setString(spellNames[chargeMagicType]);
    nextSpell.setString(spellNames[Projectile::upgrade(static_cast<MAGIC_POWER>(chargeMagicType))]);
  }
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

void gx::HUD::buffLHelper(std::string & path) {
   sf::Texture* tText;
   sf::Sprite* tSprite;
   tText = new sf::Texture();
   tSprite = new sf::Sprite();
   tText->loadFromFile(path);
   tSprite->setTexture(*tText);
   tSprite->setScale(0.5,0.5);
   buffLTextures.push_back(tText);
   buffLSprites.push_back(tSprite);
   remainTime.push_back(0);
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
}

void gx::HUD::updateHUDTimer(float timer) {
  this->timer = timer;
}

void gx::HUD::initializeSprites() {
   //buff
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
   buffHelper(std::string("graphics/Images/stunT.png"));
   buffHelper(std::string("graphics/Images/statT.png"));
   buffHelper(std::string("graphics/Images/statG.png"));
   buffHelper(std::string("graphics/Images/stunG.png"));
   buffHelper(std::string("graphics/Images/stunI.png"));
   buffHelper(std::string("graphics/Images/powerDef.png"));
   
   //buffL
   buffLTextures.push_back(new sf::Texture());
   buffLSprites.push_back(new sf::Sprite());
   remainTime.push_back(0);
   buffLHelper(std::string("graphics/Images/powerMoveL.png"));
   buffLHelper(std::string("graphics/Images/powerManaL.png"));
   buffLHelper(std::string("graphics/Images/powerHealthL.png"));
   buffLHelper(std::string("graphics/Images/powerStrL.png"));
   buffLHelper(std::string("graphics/Images/powerAttackL.png"));
   buffLHelper(std::string("graphics/Images/powerChargeL.png"));
   buffLHelper(std::string("graphics/Images/statFL.png"));
   buffLHelper(std::string("graphics/Images/statIL.png"));
   buffLHelper(std::string("graphics/Images/stunTL.png"));
   buffLHelper(std::string("graphics/Images/statTL.png"));
   buffLHelper(std::string("graphics/Images/statGL.png"));
   buffLHelper(std::string("graphics/Images/stunGL.png"));
   buffLHelper(std::string("graphics/Images/stunIL.png"));
   buffLHelper(std::string("graphics/Images/powerDefL.png"));


   //waepon
   weaponTextures.push_back(new sf::Texture());
   weaponSprites.push_back(new sf::Sprite());
   weaponHelper(std::string("graphics/Images/weaponFire.png"));
   weaponHelper(std::string("graphics/Images/weaponIce.png"));
   weaponHelper(std::string("graphics/Images/weaponThu.png"));
   weaponHelper(std::string("graphics/Images/weaponFist.png"));
   weaponHelper(std::string("graphics/Images/weaponBasic.png"));
}

