#include "HUD.h"
#include <string>
#include "Projectile.h"
#include <math.h>

/* TODO the constants should really be defined somewhere */
gx::HUD::HUD(void):health(100), maxHealth(100), HLossPercentage(0), 
  mana(100), maxMana(100), MLossPercentage(0), canPickUp(false),
  weapon1(0), weapon2(0), currentSelect(0), elapsedChargeTime(0),
  totalChargeTime(-1), chargeMagicType(0), charging(false), timer(0),
  aimerOuter(0), aimerInner(0), playerDirection(0,0,0), hit(0), attackedAngle(0),
  switched(false), miniMapProx(0), doMiniMap(false){
  font.loadFromFile("MORPHEUS.TTF");
  //bad guy icon
  badGuyTexture.loadFromFile("graphics/Images/BG_Icon.png");
  badGuyTexture.setSmooth(true);
  badGuySprite.setTexture(badGuyTexture);

  //good guy icon
  goodGuyTexture.loadFromFile("graphics/Images/GG_Icon.png");
  goodGuyTexture.setSmooth(true);
  goodGuySprite.setTexture(goodGuyTexture);
  
  //emptybar
  emptyBarTexture.loadFromFile("graphics/Images/emptybar.png");
  //heart image
  heartTexture.loadFromFile("graphics/Images/health_icon.png");
  heartSprite.setTexture(heartTexture);
  heartSprite.setPosition(10+badGuyTexture.getSize().x*2/3.0,10+22.5);
  //mana image
  manaTexture.loadFromFile("graphics/Images/mana_icon.png");
  manaSprite.setTexture(manaTexture);
  manaSprite.setPosition(10+badGuyTexture.getSize().x*2/3.0,15+heartTexture.getSize().y+22.5);
  //health bar empty 
  hEmptyBarSprite.setTexture(emptyBarTexture);
  hEmptyBarSprite.setPosition(badGuyTexture.getSize().x*2/3.0+10+heartTexture.getSize().x-10,
    10+(heartTexture.getSize().y-emptyBarTexture.getSize().y) / 2.0+22.5);
  //heath bar 
  hBarTexture.loadFromFile("graphics/Images/life.png");
  hBarSprite.setTexture(hBarTexture);
  hBarSprite.setPosition(badGuyTexture.getSize().x*2/3.0+10+heartTexture.getSize().x-10,
    10+(heartTexture.getSize().y-emptyBarTexture.getSize().y) / 2.0+22.5);
  //heath text
  healthText.setFont(font);
  healthText.setCharacterSize(18);
  healthText.setColor(sf::Color::White);
  //mana bar empty 
  mEmptyBarSprite.setTexture(emptyBarTexture);
  mEmptyBarSprite.setPosition(badGuyTexture.getSize().x*2/3.0+10+manaTexture.getSize().x-10,
    15+ heartTexture.getSize().y+(manaTexture.getSize().y-emptyBarTexture.getSize().y) / 2.0+22.5);
  //mana bar  
  mBarTexture.loadFromFile("graphics/Images/mana.png");
  mBarSprite.setTexture(mBarTexture);
  mBarSprite.setPosition(badGuyTexture.getSize().x*2/3.0+10+manaTexture.getSize().x-10,
    15+heartTexture.getSize().y+(manaTexture.getSize().y-emptyBarTexture.getSize().y) / 2.0+22.5);
  //mana text
  manaText.setFont(font);
  manaText.setCharacterSize(18);
  manaText.setColor(sf::Color::White);
  //weapin pick up text
  pickUp.setFont(font);
  pickUp.setCharacterSize(24);
  pickUp.setColor(sf::Color::White);
  pickUp.setPosition(10,15+badGuyTexture.getSize().y);

  winnerSprite.setTexture(goodGuyTexture);
  winnerSprite.setOrigin(goodGuyTexture.getSize().x/2, goodGuyTexture.getSize().y/2);
  winnerSprite.setColor(sf::Color(255,255,255,80));

  //positionText
//  positionText.setFont(font);
//  positionText.setCharacterSize(24);
//  positionText.setColor(sf::Color::Green);
//  positionText.setPosition(300,500);
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
  currentSpell.setColor(sf::Color::White);
  nextSpell.setColor(sf::Color::White);
  clockText.setFont(font);
  clockText.setCharacterSize(36);
  clockText.setColor(sf::Color::White);
  collectedPU.setFont(font);
  collectedPU.setCharacterSize(24);
  collectedPU.setColor(sf::Color::White);
  collectedPU.setPosition(10, 10+5+5+badGuyTexture.getSize().y+30);
  //minimap
  //miniMapTexture.loadFromFile("graphics/Images/minimap.png");
  miniMapProx = aimerTextures[1]->getSize().x/2;//miniMapTexture.getSize().x/2; //this should be the radius of the minimap
  miniMapSprite.setTexture(miniMapTexture);
  miniMapSprite.setOrigin(miniMapTexture.getSize().x/2, miniMapTexture.getSize().y/2);
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
  for (auto itr=aimerSprites.begin();itr != aimerSprites.end(); itr++) {
    delete *itr;
  }
  for (auto itr=aimerTextures.begin();itr != aimerTextures.end(); itr++) {
    delete *itr;
  }
  for (auto itr=hitSprites.begin();itr != hitSprites.end(); itr++) {
    delete *itr;
  }
  for (auto itr=hitTextures.begin();itr != hitTextures.end(); itr++) {
    delete *itr;
  }
  for (auto itr=hitDirSprites.begin();itr != hitDirSprites.end(); itr++) {
    delete *itr;
  }
  for (auto itr=hitDirTextures.begin();itr != hitDirTextures.end(); itr++) {
    delete *itr;
  }
  for (auto itr=playerSprites.begin();itr != playerSprites.end(); itr++) {
    delete *itr;
  }
}

void gx::HUD::draw(sf::RenderWindow & window) {
  float passed = hitClock.getElapsedTime().asSeconds();
  float winX = window.getSize().x;
  float winY = window.getSize().y;

  if (passed< 1.5) {
    hitSprites[hit]->setScale(static_cast<float>(window.getSize().x)/hitTextures[hit]->getSize().x,
    static_cast<float>(window.getSize().y)/hitTextures[hit]->getSize().y);
    sf::Color old = hitSprites[hit]->getColor();
    hitSprites[hit]->setColor(sf::Color(old.r,old.g,old.b, (1-passed/1.5)*255 ));
    window.draw(*(hitSprites[hit]));

    hitDirSprites[hit]->setColor(sf::Color(old.r,old.g,old.b, (1-passed/1.5)*255 ));
    hitDirSprites[hit]->setPosition((window.getSize().x)/2, (window.getSize().y)/2);
    hitDirSprites[hit]->setRotation(attackedAngle);
    window.draw(*(hitDirSprites[hit]));
  }
  if (buffClock.getElapsedTime().asSeconds() <1.5) {
   // collectedPU.setString(std::string("Blessed with ") + powerUpNames[ptype]);
   // window.draw(collectedPU);
  }
  std::string healthS(std::to_string(static_cast<long long>(health)) + 
    std::string("/") +std::to_string(static_cast<long long>(maxHealth)));
  healthText.setString(healthS);
  healthRect = manaText.getGlobalBounds();
  healthText.setPosition(badGuyTexture.getSize().x*2/3.0+10+heartTexture.getSize().x-10+ 
    (emptyBarTexture.getSize().x -healthRect.width)/2 , 
    10+ 25+(heartTexture.getSize().y-emptyBarTexture.getSize().y)/2.0);
  //TODO: use stringstrem here
  std::string manaS(std::to_string(static_cast<long long>(mana)) + 
    std::string("/") +std::to_string(static_cast<long long>(maxMana)));
  manaText.setString(manaS);
  manaRect = manaText.getGlobalBounds();
  manaText.setPosition( badGuyTexture.getSize().x*2/3.0+10+manaTexture.getSize().x -10+
    (emptyBarTexture.getSize().x-manaRect.width)/2 , 
    15+ 25+ heartTexture.getSize().y+ (heartTexture.getSize().y-emptyBarTexture.getSize().y)/2.0);
  //draw
  if (minotaur) 
    window.draw(badGuySprite);
  else 
    window.draw(goodGuySprite);
  window.draw(hEmptyBarSprite);
  window.draw(mEmptyBarSprite);
  window.draw(hBarSprite);
  window.draw(mBarSprite);
  window.draw(healthText);
  window.draw(manaText);
//  window.draw(positionText);
  window.draw(heartSprite);
  window.draw(manaSprite);
  badGuySprite.setPosition(10,10);
  goodGuySprite.setPosition(10,10);
  if (canPickUp) 
    window.draw(pickUp);

  //draw aimer
  //vector of sprites
  //set an int based on which ones to draw
  //update[aimerIndex]
  if (charging) {
    aimerSprites[aimerOuter]->setPosition((window.getSize().x)/2, (window.getSize().y)/2);
    aimerSprites[aimerInner]->setPosition((window.getSize().x)/2, (window.getSize().y)/2);
    aimerSprites[aimerOuter]->setRotation((elapsedChargeTime/totalChargeTime)*360);
    aimerSprites[aimerInner]->setRotation((totalChargeTime - elapsedChargeTime)/totalChargeTime *360);
    window.draw(*aimerSprites[aimerInner]);
    window.draw(*aimerSprites[aimerOuter]);
  } else if( timer <= 0 ){//only display this when in the actual game not during countdown

  //AIMER switch pivot calculation
    float pivotX = winX/2;
    float dg =atan2(winX/2,winY/2); 
    static float pivotY = -tan(2*dg- M_PI/2)*winX/2; 
    static float theta = M_PI/90;
    static float currAngle = M_PI/2;
    float radius = winY/2+tan(2*dg- M_PI/2)*winX/2;
    float startX = aimerSprites[1]->getPosition().x;
    float startY = aimerSprites[1]->getPosition().y;

    //start switching
    if(switched){
      currAngle = 0;
      startX = pivotX + radius;
      startY = -tan(2*dg- M_PI/2)*winX/2;
    }
	//switch in calculation
    if ( M_PI/2 - currAngle > 1.0E-8){
      float tmpX = winX/2+radius - (radius-cos(currAngle)*radius); 
      float tmpY = -tan(2*dg- M_PI/2)*winX/2 + sin(currAngle)*radius; 
      startX = tmpX;
      startY = tmpY;
      currAngle += theta;
    } else { //center of screen
      startX = winX/2;
      startY = winY/2;
    }
    aimerSprites[1]->setPosition(startX, startY);
    window.draw(*aimerSprites[1]);
  }

  int buffn = 0; 
  int debuffn = 0;
  for ( unsigned int i =0; i<renderBuff.size(); i++ ) {
    if (renderBuff[i]) {
      if (i>=7 && i <=13) {
        buffSprites[i]->setPosition(badGuyTexture.getSize().x+debuffn*52.5,window.getSize().y-150+55);
        debuffn++;
      } else {
        buffSprites[i]->setPosition(badGuyTexture.getSize().x+buffn*52.5,window.getSize().y-150);
        buffn++;
      }
//      buffLSprites[i]->setPosition(70+buffn*37,window.getSize().y-150);
      if (remainTime[i]<1000 && (remainTime[i]/100 % 2)){
        //window.draw(*buffLSprites[i]);
      }
      else 
        window.draw(*buffSprites[i]);
      }
  }
  //draw selected weapon
  int index1 = (currentSelect) ? weapon2 : weapon1;
  int index2 = (currentSelect) ? weapon1 : weapon2;
  weaponSprites[index1]->setPosition(window.getSize().x-20-
    weaponTextures[index1]->getSize().x*2/3.0-weaponTextures[index2]->getSize().x/3.0, 
    10+weaponTextures[index2]->getSize().y/3.0*0.707);
  weaponSprites[index1]->setScale(2/3.0,2/3.0);
  window.draw(*weaponSprites[index1]);
  weaponSprites[index2]->setPosition(window.getSize().x-20-
    weaponTextures[index2]->getSize().x/3.0*1.707,10);
  weaponSprites[index2]->setScale(1/3.0,1/3.0);
  window.draw(*weaponSprites[index2]); 
  weaponSprites[index1]->setScale(1,1);
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

  if (timer > 0 ) {
    winnerSprite.setPosition(window.getSize().x/2, window.getSize().y/2);
    clockText.setString(std::string("Game starts in ") +
      std::to_string(static_cast<long long>(timer)) +
      std::string(" seconds"));
    clockText.setPosition((window.getSize().x -clockText.getGlobalBounds().width)/2,200);
    window.draw(clockText);
    window.draw(winnerSprite);    
  }

  //minimap
  if(doMiniMap){
    //float centerMiniMapX = winX - miniMapProx;
    //float centerMiniMapY = winY - miniMapProx;
    //miniMapSprite.setPosition(centerMiniMapX, centerMiniMapY);
    //window.draw(miniMapSprite);
    for( unsigned int i = 0; i < playerSprites.size(); i++){
       //recalculate miniMapX
       vector3f v = playerPositions[i];
       v += vector3f(winX/2, winY/2,0); //this should be += center of minimap
 
      playerSprites[i]->setPosition( v.x, v.y );
      window.draw(*(playerSprites[i]));
    }
  }
}

void gx::HUD::updateHUD(int id, const std::vector<Player>& players) {
  const Player & player = players[id];
  HLossPercentage = (maxHealth-player.getHealth()) / maxHealth;
  MLossPercentage = (maxMana-player.getMana()) / maxMana;
  health = player.getHealth();
  mana = player.getMana();
  minotaur = player.isMinotaur();
  canPickUp = (player.getPickupWeaponType() != NONEWEAPON); 
  if (canPickUp) pickUp.setString("Hold F to pick up " + WeaponNames[player.getPickupWeaponType()]);
  hBarSprite.setTextureRect(sf::IntRect(0,0, static_cast<int>(health/maxHealth*emptyBarTexture.getSize().x), 40));
  mBarSprite.setTextureRect(sf::IntRect(0,0, static_cast<int>(mana/maxMana*emptyBarTexture.getSize().x), 40));
  if (totalChargeTime)
    energeBarSprite.setTextureRect(sf::IntRect(0,0,static_cast<int>(elapsedChargeTime/totalChargeTime*400),15));  

  //positionText.setString(std::to_string(static_cast<long double>(player.getPosition().x))+" , "+
//    std::to_string(static_cast<long double>(player.getPosition().y))+" , "+
//    std::to_string(static_cast<long double>(player.getPosition().z)));  
 
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
  switched = false;
  if(currentSelect != player.getCurrentWeaponSelection()){
    switched = true;
  }
  currentSelect = player.getCurrentWeaponSelection();
  this->elapsedChargeTime = player.elapsedChargeTime;
  this->totalChargeTime = player.totalChargeTime;
  this->chargeMagicType = player.chargeMagicType;
  //std::cout<<"charge time is " << player.elapsedChargeTime <<std::endl;
  //std::cout<<"total time is " << player.totalChargeTime <<std::endl;
  //std::cout<<"magic type is " << player.chargeMagicType <<std::endl;
  charging = player.charging;
  //spell
  if (chargeMagicType>=0) {//has a proj charging
    aimerOuter = aimerIndex[chargeMagicType][0];
    aimerInner = aimerIndex[chargeMagicType][1];
    currentSpell.setString(spellNames[chargeMagicType]);
    nextSpell.setString(spellNames[Projectile::upgrade(static_cast<MAGIC_POWER>(chargeMagicType))]);
  }
  if (player.attacked) {
    hitClock.restart(); 
    hit = hitIndex[player.attackedMagicType];
    attackedDir = player.attackedDir;
  }

  attackedAngle = 180 - rotateAngle(playerDirection, attackedDir);

  if (player.collectPowerUp) {
    buffClock.restart();
    this->ptype = player.ptype;
  }

  //minimap
  if( player.isMinotaur() || StringToNumber<int>(ConfigManager::configMap["minotaurRadarOnly"])==0){
    doMiniMap = true;
    static float miniMapScaling = 0.5; //bigger number means players appear farther away
    playerPositions.clear();
    vector3f myPos = players[id].getPosition();

    for( auto playerP = players.begin(); playerP != players.end(); playerP++){
      vector3f playerVec = playerP->getPosition() - myPos;
    
      float thetaBA = rotateAngleRad(playerDirection, playerVec);
      vector3f playerRes(sin(thetaBA),cos(thetaBA),0);
      playerRes.scale(playerVec.magnitude()*miniMapScaling);
      playerRes.negate();
      if(playerRes.magnitude() > miniMapProx){
        playerRes.normalize();
        playerRes.scale(miniMapProx);
      }
      playerPositions.push_back(playerRes);
    }
  } else {
    doMiniMap = false;
  }
}

void gx::HUD::buffHelper(std::string & path) {
   sf::Texture* tText;
   sf::Sprite* tSprite;
   tText = new sf::Texture();
   tSprite = new sf::Sprite();
   tText->loadFromFile(path);
   tText->setSmooth(true);
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
 //  tSprite->setScale(0.5,0.5);
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
   tText->setSmooth(true);
   tSprite->setTexture(*tText);
   weaponTextures.push_back(tText);
   weaponSprites.push_back(tSprite);
}

void gx::HUD::aimerHelper(std::string & path) {
   sf::Texture* tText;
   sf::Sprite* tSprite;
   tText = new sf::Texture();
   tSprite = new sf::Sprite();
   tText->loadFromFile(path);
   tSprite->setTexture(*tText);
   tSprite->setOrigin(tText->getSize().x/2, tText->getSize().y/2);
   aimerTextures.push_back(tText);
   aimerSprites.push_back(tSprite);
}

void gx::HUD::hitHelper(std::string & path) {
   sf::Texture* tText;
   sf::Sprite* tSprite;
   tText = new sf::Texture();
   tSprite = new sf::Sprite();
   tText->loadFromFile(path);
   tSprite->setTexture(*tText);
   hitTextures.push_back(tText);
   hitSprites.push_back(tSprite);
}

void gx::HUD::hitDirHelper(std::string & path) {
   sf::Texture* tText;
   sf::Sprite* tSprite;
   tText = new sf::Texture();
   tSprite = new sf::Sprite();
   tText->loadFromFile(path);
   tSprite->setTexture(*tText);
   tSprite->setOrigin(tText->getSize().x/2, tText->getSize().y);
   hitDirTextures.push_back(tText);
   hitDirSprites.push_back(tSprite);
}

void gx::HUD::updateHUDTimer(float timer) {
  this->timer = timer;
}

void gx::HUD::initializeSprites() {
   //buff
   buffTextures.push_back(new sf::Texture());
   buffSprites.push_back(new sf::Sprite());
   renderBuff.push_back(false);
   buffHelper(std::string("graphics/Images/speed_boost.png"));
   buffHelper(std::string("graphics/Images/mana_regen.png"));
   buffHelper(std::string("graphics/Images/health_regen.png"));
   buffHelper(std::string("graphics/Images/attack_boost.png"));
   buffHelper(std::string("graphics/Images/powerAttack.png"));
   buffHelper(std::string("graphics/Images/attack_speed.png"));
   buffHelper(std::string("graphics/Images/burn_debuff.png")); //burn
   buffHelper(std::string("graphics/Images/ice_debuff.png")); //ice slow
   buffHelper(std::string("graphics/Images/thunder_stun.png")); //stun thunder
   buffHelper(std::string("graphics/Images/thunder_debuff.png")); //thunder debuff
   buffHelper(std::string("graphics/Images/dark_debuff.png"));  //dark 1
   buffHelper(std::string("graphics/Images/dark_stun.png"));  //dark 2 (stun)
   buffHelper(std::string("graphics/Images/ice_stun.png"));   //frozen
   buffHelper(std::string("graphics/Images/defenseup.png")); 
   
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
   weaponHelper(std::string("graphics/Images/dark_icon.png"));
   weaponHelper(std::string("graphics/Images/fire_icon.png"));
   weaponHelper(std::string("graphics/Images/ice_icon.png"));
   weaponHelper(std::string("graphics/Images/thunder_icon.png"));
   weaponHelper(std::string("graphics/Images/fist_icon.png"));
   weaponHelper(std::string("graphics/Images/basic_icon.png"));

   //aimer
   aimerHelper(std::string("graphics/Images/aimerEmpty.png"));  //0
   aimerHelper(std::string("graphics/Images/aimerNone.png"));
   aimerHelper(std::string("graphics/Images/aimerF1O.png"));  //2
   aimerHelper(std::string("graphics/Images/aimerF1I.png"));  
   aimerHelper(std::string("graphics/Images/aimerF2O.png"));  
   aimerHelper(std::string("graphics/Images/aimerF3O.png"));  //5
   aimerHelper(std::string("graphics/Images/aimerF3I.png"));
   aimerHelper(std::string("graphics/Images/aimerI1O.png"));  //7
   aimerHelper(std::string("graphics/Images/aimerI1I.png"));  
   aimerHelper(std::string("graphics/Images/aimerI2O.png"));  
   aimerHelper(std::string("graphics/Images/aimerI3O.png"));  //10
   aimerHelper(std::string("graphics/Images/aimerI3I.png"));
   aimerHelper(std::string("graphics/Images/aimerT1O.png"));  //12
   aimerHelper(std::string("graphics/Images/aimerT1I.png"));  
   aimerHelper(std::string("graphics/Images/aimerT2O.png"));  
   aimerHelper(std::string("graphics/Images/aimerT3O.png"));  //15
   aimerHelper(std::string("graphics/Images/aimerT3I.png"));  //16
   aimerHelper(std::string("graphics/Images/aimerG1.png"));
   aimerHelper(std::string("graphics/Images/aimerG2.png"));
   aimerHelper(std::string("graphics/Images/aimerG3.png"));
   aimerHelper(std::string("graphics/Images/aimerBasic.png")); //20

   hitTextures.push_back(new sf::Texture());
   hitSprites.push_back(new sf::Sprite());
   hitHelper(std::string("graphics/Images/hitFire.png"));
   hitHelper(std::string("graphics/Images/hitIce.png"));
   hitHelper(std::string("graphics/Images/hitThu.png"));
   hitHelper(std::string("graphics/Images/hitGravity.png"));
   hitHelper(std::string("graphics/Images/hitBasic.png"));

   hitDirTextures.push_back(new sf::Texture());
   hitDirSprites.push_back(new sf::Sprite());
   hitDirHelper(std::string("graphics/Images/fireHitDir.png"));
   hitDirHelper(std::string("graphics/Images/iceHitDir.png"));
   hitDirHelper(std::string("graphics/Images/thuHitDir.png"));
   hitDirHelper(std::string("graphics/Images/gravityHitDir.png"));
   hitDirHelper(std::string("graphics/Images/basicHitDir.png"));

   miniMapBlipTexture.loadFromFile("graphics/Images/blip.png");
   for( int i = 0 ; i < StringToNumber<int>(ConfigManager::configMap["players"]); i++){
     sf::Sprite* spritePtr = new sf::Sprite();
     spritePtr->setTexture(miniMapBlipTexture);
     spritePtr->setOrigin(miniMapBlipTexture.getSize().x/2, miniMapBlipTexture.getSize().y/2);
     playerSprites.push_back(spritePtr);
   }

}

void gx::HUD::setWinner(Game_State w)
{
  winnerSprite.setColor(sf::Color(255,255,255,80));
  if(w == MANOTAUR_WIN)
  {
    winnerSprite.setTexture(badGuyTexture);
    winnerSprite.setOrigin(badGuyTexture.getSize().x/2, badGuyTexture.getSize().y/2);
  }
  else if( w == CIVILIAN_WIN )
  {
    winnerSprite.setTexture(badGuyTexture);
    winnerSprite.setOrigin(badGuyTexture.getSize().x/2, badGuyTexture.getSize().y/2);
  }
  else
  {
    winnerSprite.setColor(sf::Color(255,255,255,0));
  }
}

void gx::HUD::updateDir(vector3f & dir){
  playerDirection = dir;
}

float gx::HUD::rotateAngle( vector3f v1, vector3f v2 ){
  return static_cast<float>(atan2(v1.x*v2.y-v2.x*v1.y,v1.x*v2.x+v1.y*v2.y)) * 180/M_PI ;
}

float gx::HUD::rotateAngleRad( vector3f v1, vector3f v2 ){
  return static_cast<float>(atan2(v1.x*v2.y-v2.x*v1.y,v1.x*v2.x+v1.y*v2.y));
}

const int gx::HUD::hitIndex[18] = {
  1,
  1,
  1,
  2,
  2,
  2,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  5
};


//outer inner
const int gx::HUD::aimerIndex[18][2] = {
  //FIR1=0, FIR2, FIR3, 
  { 2, 3 },
  { 4, 3 },
  { 5, 6 },
  //ICE1, ICE2, ICE3,
  { 7, 8 },
  { 9, 8 },
  { 10, 11 },
  //THU1, THU2, THU3,
  { 12, 13 },
  { 14, 13 },
  { 15, 16 },
  //G_IT, G_FT, G_FI, //gravity and what it is missing
  { 17, 0 },
  { 17, 0 },
  { 17, 0 },
  //G2,
  { 18, 0 },
  //G_IT2, G_FT2, G_FI2,
  { 17, 0 },
  { 17, 0 },
  { 17, 0 },
  //G3,
  { 19, 0 },
  //B1
  { 20, 0 }
};