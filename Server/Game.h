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
  public:
  static const int packetType = STARTGAME;
  StartGamePacket() {}

  void serialize(sf::Packet& ) const{
  }

  void deserialize(sf::Packet& ){
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

