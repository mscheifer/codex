#pragma once
#include "Map.h"
#include "ServerGameTimeRespond.h"
#include "ClientGameTimeAction.h"
#include "StaticEnums.h"
#include <time.h>
#include <stdlib.h> /* srand rand */
#include <time.h>   /* time */

struct IdPacket
{
public:
  static const int packetType = JOINID;
  int id;
  IdPacket(int i)
  {
    id = i;
  }

  void serialize(sf::Packet& packet)
  {
    packet<<id;
  }

  void deserialize(sf::Packet& packet)
  {
    packet>>id;
  }
};

class Game
{
public:
	Game(void);
	Game(Map m);
	ServerGameTimeRespond evaluate(ClientGameTimeAction a);
	~Game(void);
  int join();
  void chooseMinotaur();
private:
	Map world;
};

