#pragma once
#include "Map.h"
#include "ServerGameTimeRespond.h"
#include "ClientGameTimeAction.h"
#include "StaticEnums.h"
#include <time.h>
#include <stdlib.h> /* srand rand */
#include <time.h>   /* time */
#include "StaticEntity.h"

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
  void addPlayer() {
    playerStatus.push_back(std::pair<int,bool>(playerStatus.size(),false));
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
  std::vector<StaticEntity*> staticEntities;


  InitPacket() {}
  InitPacket(int idc, v3_t coord, v3_t dir, std::vector<StaticEntity*> e ) : id(idc), position(coord), direction(dir), staticEntities(e) {}

  void serialize(sf::Packet& packet) const{
    packet << id;
    position.serialize(packet);
    direction.serialize(packet);
    sf::Uint32 size = static_cast<sf::Uint32>(staticEntities.size());
    packet << size;
    for (unsigned int i = 0; i < staticEntities.size();i++) { 
      staticEntities[i]->serialize(packet);
    }
  }

  void deserialize(sf::Packet& packet){
    packet >> id;
    position.deserialize(packet);
    direction.deserialize(packet);
    sf::Uint32 size;
    packet >> size;
    for (unsigned int i = 0; i < size; i++) { 
      StaticEntity* staticEntity = new StaticEntity(); 
      staticEntity->deserialize(packet);
      this->staticEntities.push_back(staticEntity);
    }
  }
};

class Game
{
public:
	Game(void);
	Game(Map m);
	void evaluate(ClientGameTimeAction a);
	void prepResponse(ServerGameTimeRespond& sgtr);
  void updateAndResolveCollision(void);
	~Game(void);
  int join();
  void chooseMinotaur();
  InitPacket getInitPacket(int playerId);
  void clearEvents();
  void restartGame();
  void initScores();
  void assignName(std::string name, int id);
private:
	Map world;
};

