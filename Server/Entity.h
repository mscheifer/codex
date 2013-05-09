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
	  std::vector<std::pair<Entity*,gx::vector3f>> entities =  detectCollision();
	  Coordinate c = getPosition() ;
	  for(unsigned int i = 0; i < entities.size() ; i ++) {
      Entity* e = entities[i].first;
		  gx::vector3f fixShit = entities[i].second;
      std::cout<< "was at " << c.x << ","<< c.y << "," << c.z << std::endl;
      //why the fuck does commenting out the below line break everything?
      std::cout<< "fix at " << fixShit.x << ","<< fixShit.y << "," << fixShit.z << "mag: " << fixShit.magnitude() << std::endl;
		  c.x += fixShit.x;
		  c.y += fixShit.y;
		  c.z += fixShit.z;
      //std::cout << "collided fuck" <<std::endl;
      std::cout<< "now at " << c.x << ","<< c.y << "," << c.z << std::endl;
	  }
	  setPosition(c);
    updateBounds();
  }
  virtual bool isProjectile(void){ return false;}
  virtual bool isWeapon(void){ return false;}
  virtual bool isPlayer(void){ return false;}
  
  //update the bounding objects
  //the bounding objects should automatically update thier position on the tree
  virtual void updateBounds(){}
  std::vector<std::pair<Entity*,gx::vector3f>> detectCollision();

  Coordinate getPosition(void) const { return position; }
  Direction getDirection(void) const { return direction; }
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
