#pragma once
#include "Entity.h"
#include "Player.h"

class PowerUp : public Entity{
public:
  PowerUp(){} ;
  PowerUp(v3_t p, Map* m) ;
  ~PowerUp() {};
  
  static const Entity_Type type = POWER_UP;
  //each different power up @alvin @allen
  void serialize(sf::Packet& packet) const;

  void deserialize(sf::Packet& packet);
  void handleCollisions();
  void update();
  void updateBounds(); //TODO this maybe should be in inherited (if differen sizes)

  Entity_Type getType() const {
    return type;
  }

protected:
  sf::Clock Respown_Counter;
  int Respown_Time;

private:
	float healthMutiplyer;
	float manaMutiplyer;
	float speedMutiplyer;
	bool active;
};
