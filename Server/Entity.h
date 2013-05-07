#pragma once
#include "3DCoordinate.h"
#include "directionalVector.h"
#include "ClientGameTimeAction.h"
#include "Physics.h"
#include "Map.h"
#include <vector>
#include "boundingManager.h"

class Entity{
protected:
  Coordinate position;
  Direction direction;
  Map* map;
  std::vector<BoundingObj*> boundingObjs;
  // Some kind of state {paralyzed, frozen, blah blah}
  // Power ups {contains MULTIPLERS for health, defense/ elemental weapons}

public:
  Entity() {}
  ~Entity() {}
  
  virtual void handleAction(ClientGameTimeAction){}
  virtual void update(){}
  virtual void onCollision(){  
	  
	std::vector<std::pair<Entity*,gx::vector3>> entities =  detectCollision();
	Coordinate c = getPosition() ;
	for(int i = 0; i < entities.size() ; i ++) {
		gx::vector3 fixShit = entities[i].second;
		c.x = fixShit.x;
		c.y = fixShit.y;
		c.z = fixShit.z;
	}
	setPosition(c);
  
  }
  virtual bool isProjectile(void){ return false;}
  virtual bool isWeapon(void){ return false;}
  virtual bool isPlayer(void){ return false;}

  void updateBoundsOnTree();
  std::vector<std::pair<Entity*,gx::vector3>> detectCollision();
  virtual void updateBounds(){}

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

  void setBoundingObjs(std::vector<BoundingObj*> b){
    boundingObjs = b;
  }

  std::vector<BoundingObj*> getBoundingObjs(){
    return boundingObjs;
  }
};
