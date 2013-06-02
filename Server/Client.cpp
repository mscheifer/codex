#include "Client.h"
#include <iostream>
#include "AudioManager.h"
#include "Game.h"
namespace {
} //end nunnamed namespace

void NetworkClient::receiveMessages() {
  //receive from server and process
   
  sf::Packet packet;
  if (netRecv.receiveMessage(packet)) {
    sf::Uint32 packetType;
    packet >> packetType;
    std::vector<int> kills;
    std::vector<int> wins;
    std::vector<Entity*> entities;
    int proximity = 2;
    v3_t pos;
    bool minotaur;
    v3_t dir;
    static float maxProx = 30.f;
    IdPacket newId(0);
    StartGamePacket playerSt;
    switch (packetType) {
      case SGTR:
        this->s.deserialize(packet);
        if (s.state != PLAYING)
          gameRestart = true;
        pos = s.players[this->id].getPosition();
		proximity = 2;
        minotaur = s.players[this->id].isMinotaur();
        
        for(auto playerP = s.players.begin(); playerP != s.players.end(); playerP++) {
          if(playerP->player_id != this->id) {
            //make sure the SGTR stays in scope
            entities.push_back(&(*playerP));

            v3_t dist = playerP->getPosition() - pos; //audio prox calculation
            if(std::abs(dist.magnitude()) >= maxProx){
              proximity--;
            } else if (playerP->isMinotaur())
              minotaur = true;
          }
          kills.push_back((*playerP).kills);
          wins.push_back((*playerP).wins);
          AudioManager::processPlayerSound(*playerP);
        }
        for(auto entP = s.walls.begin(); entP != s.walls.end(); entP++) {
          entities.push_back(*entP);
        }
     //   std::cout << "new pack" << std::endl;
        for(auto entP = s.projectiles.begin(); entP != s.projectiles.end(); entP++) {
     //     std::cout << "there's a projectile at (" << (**entP).getPosition().x << "," << (**entP).getPosition().y << "," << (**entP).getPosition().z << ")" << std::endl;
          entities.push_back(*entP);
           AudioManager::processProjectileSound(**entP);
        }
        for(auto entP = s.powerups.begin(); entP != s.powerups.end(); entP++) {
          entities.push_back(*entP);
        }
        for(auto entP = s.weapons.begin(); entP != s.weapons.end(); entP++) {
          entities.push_back(*entP);
        }
        
        gxClient.updatePosition(gx::vector4f(pos.x,pos.y,pos.z));
        //entities.push_back(&(this->skybox)); //add skybox
        gxClient.updateEntities(entities);
        gxClient.updateHUD(s.players[id]);
        gxClient.updateScores(wins,kills);
        //std::cout << "num entities received: " << entities.size() << std::endl;
        if (s.players[id].dead) { /*render death everytime ? */}
        //render WIN OR LOSE based on s.state
        sf::Listener::setPosition(pos.x/AudioManager::soundScaling, pos.y/AudioManager::soundScaling, pos.z/AudioManager::soundScaling);
        dir = s.players[this->id].getDirection();
        sf::Listener::setDirection(dir.x, dir.y, dir.z);

        //TODO not sure where to put this @bowen add to HUD here
        if( s.players[id].getPickupWeaponType() != UNK )
          std::cout << "can pick up weapon type " << WeaponNames[s.players[id].getPickupWeaponType()] << std::endl;
        
        //calculate proximity of players
        //can do -- instead of ++ so we can have more intense if less players
        
        //if(s.players[this->id].getPosition().y > -5){
        //  proximity++;
        //}
        //if(s.players[this->id].getPosition().y > 5){
        //  proximity++;
        //}

        //if(s.players[this->id].getPosition().x > 0){
        //  minotaur = false;
        //}
        //std::cout<<"prox " << proximity << "mino " << minotaur << std::endl;
        AudioManager::updateMusic(proximity, minotaur);
        
        break;
      case JOINID:
          newId.deserialize(packet);
          this->id = newId.id;
          std::cout << "USERID: " << this->id << std::endl;
          this->action.player_id = id;
          break;
      case STARTGAME:
          joined = true; 
          std::cout<<"CLIENT RECEIVED START GAME"<<std::endl;
          playerSt.deserialize(packet);
          gxClient.updateLobby(playerSt.playerStatus);
          for (auto itr= playerSt.playerStatus.begin(); itr != playerSt.playerStatus.end(); itr++ ) {
            std::cout<<"Player "<< (*itr).first<< " is "<<(*itr).second<<std::endl;
          }
          break;
      case INIT:
          //TODO initialize the player info
          gameStart = true;
          break;
      default:
        std::cout<<"There is an error when receiving"<<std::endl;
        break;
    }
  }
}

void NetworkClient::processInput() {
  action = this->gxClient.handleInput();
  if(this->gxClient.closed()) { //add running  in ClientGameTimeAction ?
    this->running = false;
  }
  if (action.updated) {
    action.player_id = id;
    this->sendPacket = true; 
    //ConfigManager::log(action.toString()); 
  }
}
/*
void NetworkClient::processInput(){
  bool updateAS = false;
  action.clear();
  
  if (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyReleased){        
        switch(event.key.code){
        case sf::Keyboard::Return:
          if(!chat.isTyping()){ //start typing
            chat.setBuffer("");
          }
          else{ //done typing
            sf::Packet packet;
            std:: string chatBuffer = chat.getBuffer();
            if(chatBuffer.size() > 0)
              netRecv.sendPacket<ChatObject>(ChatObject(chatBuffer));
            chat.setBuffer("");
          }
          chat.revertTyping();
          break;
        }
      }

      if (event.type == sf::Event::TextEntered){
        if( event.text.unicode >= 32 && event.text.unicode <= 125 && chat.isTyping() && chat.getBuffer().size() < 99)
          chat.appendBuffer((char) event.text.unicode);
        if (event.text.unicode == 8 &&chat.isTyping())
          chat.backspace();
      }
    }
  }

  if(updateAS && !s.players[id].dead){ //player should still be able to chat?
    sendPacket = true;
  }
}
*/
void NetworkClient::doClient() {
  AudioManager::loadSounds();

  sf::VideoMode m = sf::VideoMode::getDesktopMode();
  std::cout << "width " << m.width << std::endl;
  std::cout << "height " << m.height << std::endl;
  //AudioManager::playMusic("m1");
  //if doClient running I already connected to the server 
  gameStart = false;
  joined = false;  //joined is used for receiving game start from server
  bool clickedButton = false;
  //lobby code
  while(true) {
    sf::Packet initPacket;
    if (joined && this->gxClient.gameStart()) {
      //received start game and clicked
      initPacket << static_cast<sf::Uint32>(INIT); 
      if (!clickedButton) { 
        initPacket << true;
        netRecv.sendMessage(initPacket);
        clickedButton = true;
      } else {
        initPacket << false;
        netRecv.sendMessage(initPacket);
        clickedButton = false;
      }
    }
    initPacket.clear();
    this->receiveMessages();
    if (gameStart) break;
    this->gxClient.drawLobby();
  }
  this->gxClient.disableCursor();
  //game loop
  sf::Clock clock;
  clock.restart();
  gameRestart = true;
  while(this->running) {
    if(gameStart) { 
    /*sf::Clock profilerTime;
    float processInputTime;
    float receiveMessagesTime;
    float drawTime;
    float sendPackTime;*/
    if (!gameRestart) {
      //profilerTime.restart();
      this->processInput();
      //processInputTime = profilerTime.getElapsedTime().asMilliseconds();
      //profilerTime.restart();
      this->receiveMessages();
      //receiveMessagesTime = profilerTime.getElapsedTime().asMilliseconds();

      //profilerTime.restart();
      //window closed
      if (!this->running) 
        break;
      if (gameRestart) {
        gameStart = false;
        clock.restart();
        continue;
      }
      this->gxClient.draw();
      //drawTime = profilerTime.getElapsedTime().asMilliseconds();
      //profilerTime.restart();
      if(this->sendPacket) {
        //this->action.print();
        this->netRecv.sendPacket<ClientGameTimeAction>(action);
        this->sendPacket = false;
      }
      //sendPackTime = profilerTime.getElapsedTime().asMilliseconds();
      //std::cout<<"processInput: "<< processInputTime <<"ms\treceiveMessagesTime: "<<
      //receiveMessagesTime <<"ms\tdrawTime: "<< drawTime <<"ms\tsendPackTime: "<< sendPackTime <<std::endl;
    } else {
      float remaining = 5-clock.getElapsedTime().asSeconds();
      if (remaining <= 0) {
        gameRestart = false;
      }
      gxClient.updateHUDTimer(remaining);
      this->gxClient.draw();
    }
    } else { receiveMessages();}
  }
}
