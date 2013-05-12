#pragma once
#include "3DCoordinate.h"
#include "directionalVector.h"
#include "ClientGameTimeAction.h"
#include "Physics.h"
#include "Map.h"
#include <vector>
#include <stdint.h> //uint32_t
#include "boundingManager.h"

class Entity {
public:
  static const Entity_Type type = UNDEFINED;
protected:
  v3_t position;
  v3_t acceleration;
  v3_t velocity;
  v3_t direction;
  Coordinate position2; //TODO temp
  Direction direction2;
  Map* map;
  std::vector<BoundingObj*> boundingObjs;
  // Some kind of state {paralyzed, frozen, blah blah}
  // Power ups {contains MULTIPLERS for health, defense/ elemental weapons}

public:
  Entity() {}
  virtual ~Entity() {}
  
  virtual void handleAction(ClientGameTimeAction){}
  virtual void update(){}
  virtual void handleCollisions(){}
  
  //update the bounding objects
  //the bounding objects should automatically update thier position on the tree
  virtual void updateBounds(){}
  //this is only for showing collision boxes on client, should never be called
  virtual void updateBoundsSoft(){}
  std::vector<std::pair<Entity*,gx::vector3f>> detectCollision();

  v3_t getPosition(void) const { return position; }
  void setPosition(v3_t c) { position = c;}
  v3_t getDirection(void) const { return direction; }
  void setDirection(v3_t d) { direction = d; }
  void setVelocity(v3_t v) { velocity = v; }
  void setMap(Map* m) { map = m; }
  void setBoundingObjs(std::vector<BoundingObj*> b){ boundingObjs = b; }
  std::vector<BoundingObj*> getBoundingObjs(){ return boundingObjs; }
  
  //TODO temporary to support old stuff
  Coordinate getPosition2(void) const { return position2; }
  Direction getDirection2(void) const { return direction2; }
  void setDirection2(Direction d) { direction2 = d; }
  void setPosition2(Coordinate c) { position2 = c;}

  virtual void serialize(sf::Packet& packet) const
  {
    position.serialize(packet);
    direction.serialize(packet);
  }

  virtual void deserialize(sf::Packet& packet)
  {    
    position.deserialize(packet);
    direction.deserialize(packet);
  }

  virtual Entity_Type getType() { return type; }
};
