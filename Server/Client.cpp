#include "Client.h"
#include <iostream>
#include "AudioManager.h"
#include "Game.h"
namespace {
} //end nunnamed namespace

void NetworkClient::receiveMessages() {
  //receive from server and process
   
  sf::Packet packet;
  int proximity = 2;
  bool minotaur = false;
  static float maxProx = ConfigManager::musicProx();
  bool updateMusic = false;
  flag = false;
  static int count = 0;


  while (!flag && netRecv.receiveMessage(packet)) {
    if(count == -1)
      count = 0;
    count++;

    //TODO might cause an endless cycle if server sends faster than the time
    //it takes to process a message. 
    sf::Uint32 packetType;
    packet >> packetType;
    std::vector<int> kills;
    std::vector<int> wins;
    v3_t pos;
    v3_t dir;
    IdPacket newId(0);
    StartGamePacket playerSt;
    InitPacket initPckt;
    switch (packetType) {
      case SGTR:
        updateMusic = true;
        this->s.deserialize(packet);
        if (s.state != PLAYING)
          gameRestart = true;
        pos = s.players[this->id].getPosition();
        proximity = 2;
        minotaur = s.players[this->id].isMinotaur();
        
        this->gxClient.clearEntities();

        for(auto playerP = s.players.begin(); playerP != s.players.end(); playerP++) {
          if(playerP->player_id != this->id) {
            //make sure the SGTR stays in scope
            this->gxClient.addEntity(&(*playerP));

            v3_t dist = playerP->getPosition() - pos; //audio prox calculation
            if(std::abs(dist.magnitude()) >= maxProx){
              proximity--;
            } else if (playerP->isMinotaur()) //not far, but minotaur
              minotaur = true;
          }
          kills.push_back((*playerP).kills);
          wins.push_back((*playerP).wins);
          AudioManager::processPlayerSound(*playerP);
        }
        for(auto entP = s.walls.begin(); entP != s.walls.end(); entP++) {
          this->gxClient.addEntity(*entP);
        }
        for(auto entP = s.projectiles.begin(); entP != s.projectiles.end(); entP++) {
          this->gxClient.addEntity(*entP);
           AudioManager::processProjectileSound(**entP);
        }
        for(auto entP = s.powerups.begin(); entP != s.powerups.end(); entP++) {
          this->gxClient.addEntity(*entP);
        }
        for(auto entP = s.weapons.begin(); entP != s.weapons.end(); entP++) {
          this->gxClient.addEntity(*entP);
        }
        
        gxClient.updatePosition(gx::vector4f(pos.x,pos.y,pos.z));
        //entities.push_back(&(this->skybox)); //add skybox
        gxClient.updateHUD(s.players[id]);
        gxClient.updateScores(wins,kills);
        //std::cout << "num entities received: " << entities.size() << std::endl;
        if (s.players[id].dead) { /*render death everytime ? */}
        //render WIN OR LOSE based on s.state
        sf::Listener::setPosition(pos.x/AudioManager::soundScaling, pos.y/AudioManager::soundScaling, pos.z/AudioManager::soundScaling);
        dir = s.players[this->id].getDirection();
        sf::Listener::setDirection(dir.x, dir.y, dir.z);
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
          this->gxClient.updateLobby(playerSt.playerStatus);
          for (auto itr= playerSt.playerStatus.begin(); itr != playerSt.playerStatus.end(); itr++ ) {
            std::cout<<"Player "<< (*itr).first<< " is "<<(*itr).second<<std::endl;
          }
          break;
      case INIT:

          //TODO initialize the player info
          initPckt.deserialize(packet);
          this->gxClient.updatePosition(gx::vector4f(0,0,0) + initPckt.position);
          this->gxClient.updateDirection(initPckt.direction);
          this->gameStart = true;
          flag = true;
        //  std::cout<<" i received init" <<std::endl;
          break;
      default:
        std::cout<<"Error client receive bad packet " << packetType << std::endl;
        break;
    }
  }
  //if(count != -1)
  //  std::cout << "packets processed " << count << std::endl;
  count = -1;

  if(updateMusic){
    //std::cout << "prox " << proximity << "mino " << minotaur << std::endl;
    AudioManager::updateMusic(proximity, minotaur);
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
    if (gameStart) {
    sf::Clock profilerTime;
    float processInputTime;
    float receiveMessagesTime;
    float drawTime;
    float sendPackTime;
      if (!gameRestart) {
        profilerTime.restart();
        this->processInput();
        processInputTime = profilerTime.getElapsedTime().asMilliseconds();
        profilerTime.restart();
        this->receiveMessages();
        receiveMessagesTime = profilerTime.getElapsedTime().asMilliseconds();

        profilerTime.restart();
        //window closed
        if (!this->running) 
          break;
        if (gameRestart) {
          clock.restart();
          gameStart = false;
          if (flag) gameStart =true;
         //std::cout<<"restarting game now" <<std::endl;
          continue;
        }
        this->gxClient.draw();
        drawTime = profilerTime.getElapsedTime().asMilliseconds();
        profilerTime.restart();
        if(this->sendPacket) {
          //this->action.print();
          this->netRecv.sendPacket<ClientGameTimeAction>(action);
          this->sendPacket = false;
        }
        sendPackTime = profilerTime.getElapsedTime().asMilliseconds();
        std::cout<<"processInput: "<< processInputTime <<"ms\treceiveMessagesTime: "<<
        receiveMessagesTime <<"ms\tdrawTime: "<< drawTime <<"ms\tsendPackTime: "<< sendPackTime <<std::endl;
      } else {
        //std::cout<<" i am counting down"<<std::endl;
        float remaining = 2-clock.getElapsedTime().asSeconds(); //TODO make this 5 sec again
        //std::cout<<"remaininig tiem is " <<remaining<<std::endl;
        if (remaining <= 0) {
          gameRestart = false;
        }
        gxClient.updateHUDTimer(remaining);
        this->gxClient.draw();
      }
    } else {// std::cout<<" trying to recv init" <<std::endl;
    if (!flag) receiveMessages();}
  }
}
