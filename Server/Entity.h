#pragma once
#include "3DCoordinate.h"
#include "directionalVector.h"
#include "ClientGameTimeAction.h"
#include "Physics.h"
#include "Map.h"

class Entity{
public:
  Entity() {}
  ~Entity() {}
  
  virtual void handleAction(ClientGameTimeAction){}
  virtual void update(){}
  virtual void onCollision(Entity){}
  virtual bool isProjectile(void){ return false;}
  virtual bool isWeapon(void){ return false;}
  virtual bool isPlayer(void){ return false;}
  Coordinate getPosition(void){ return position; }
  Direction getDirection(void){ return direction; }
  void setDirection(Direction d) {
	  direction = d;
  }

  void setMap(Map* m) {
	  map = m;
  }

  void setPosition(Coordinate c) {
	  position = c;
  }
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

protected:
  Coordinate position;
  Direction direction;
  Map* map;
  // Some kind of state {paralyzed, frozen, blah blah}
  // Power ups {contains MULTIPLERS for health, defense/ elemental weapons}
};
