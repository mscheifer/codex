#pragma once
#include "Map.h"
#include "ServerGameTimeRespond.h"
#include "ClientGameTimeAction.h"
#include "StaticEnums.h"
#include <time.h>
#include <stdlib.h> /* srand rand */
#include <time.h>   /* time */

struct IdPacket{
public:
  static const int packetType = JOINID;
  int id;
  IdPacket() {}
  IdPacket(int i){ id = i;}

  void serialize(sf::Packet& packet) const{
    packet << id;
  }

  void deserialize(sf::Packet& packet){
    packet >> id;
  }
};

struct StartGamePacket //is this used?
{
  std::vector<std::pair<int,bool>> playerStatus;
  public:
  static const int packetType = STARTGAME;
  // int can be string 
  StartGamePacket() { }
  StartGamePacket(int num) { 
    for (int i =0; i < num; i++) {
      playerStatus.push_back(std::pair<int,bool>(i,false));
    }
  }
  void changeStatus(int i ) {
    playerStatus[i].second = !playerStatus[i].second;
  }

  void serialize(sf::Packet& packet ) const{
    packet << playerStatus.size();
    for (auto itr = playerStatus.begin(); itr != playerStatus.end(); itr++) {
      packet << (*itr).first;
      packet << (*itr).second;
    }
  }

  void deserialize(sf::Packet& packet ){
    int size;
    packet >> size;
    playerStatus.clear();
    int id;
    bool status;
    for (int i = 0; i< size ; i++ ) {
      packet >> id;
      packet >> status;
      playerStatus.push_back(std::pair<int,bool>(id,status));
    }
  }
};

struct InitPacket{
public:
  static const int packetType = INIT;
  int id;
  v3_t position;
  v3_t direction;

  InitPacket() {}
  InitPacket(int idc, v3_t coord, v3_t dir) : id(idc), position(coord), direction(dir){}

  void serialize(sf::Packet& packet) const{
    packet << id;
    position.serialize(packet);
    direction.serialize(packet);
  }

  void deserialize(sf::Packet& packet){
    packet >> id;
    position.deserialize(packet);
    direction.deserialize(packet);
  }
};

class Game
{
public:
	Game(void);
	Game(Map m);
	void evaluate(ClientGameTimeAction a);
	ServerGameTimeRespond prepResponse(void);
  void updateAndResolveCollision(void);
	~Game(void);
  int join();
  void chooseMinotaur();
  InitPacket getInitPacket(int playerId);
  void clearEvents();
  void restartGame();
  void initScores();
private:
	Map world;
};

