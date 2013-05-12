#pragma once
#include "Entity.h"
#include "Player.h"

class PowerUp : Entity{
public:
  PowerUp() ;
  ~PowerUp() {}
  
  //TODO will need to add extra stuff so you can render
  //each different power up @alvin @allen
  void serialize(sf::Packet& packet) const
  {
    position.serialize(packet);
    direction.serialize(packet);
  }

  void deserialize(sf::Packet& packet)
  {
    position.deserialize(packet);
    direction.deserialize(packet);
  }

  void onCollision(Entity*);
  void update();

protected:
  sf::Clock Respown_Counter;
  int Respown_Time;

private:
	float healthMutiplyer;
	float manaMutiplyer;
	float speedMutiplyer;
	bool active;
};