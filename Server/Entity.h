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
  enum Entity_Type {UNDEFINED, PROJECTILE, PLAYER, WEAPON, WALL};

protected:
  Coordinate position;
  Direction direction;
  Map* map;
  std::vector<BoundingObj*> boundingObjs;
  Entity_Type type;
  // Some kind of state {paralyzed, frozen, blah blah}
  // Power ups {contains MULTIPLERS for health, defense/ elemental weapons}

public:
  Entity() {type = UNDEFINED;}
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

  Coordinate getPosition(void) const { return position; }
  Direction getDirection(void) const { return direction; }
  void setDirection(Direction d) { direction = d; }
  void setMap(Map* m) { map = m; }
  void setPosition(Coordinate c) { position = c;}
  void setBoundingObjs(std::vector<BoundingObj*> b){ boundingObjs = b; }
  std::vector<BoundingObj*> getBoundingObjs(){ return boundingObjs; }
  Entity_Type getType(){ return type; }
  
  void serialize(sf::Packet& packet) const
  {
    position.serialize(packet);
    direction.serialize(packet);
    packet << static_cast<uint32_t>(type);
  }

  void deserialize(sf::Packet& packet)
  {    
    position.deserialize(packet);
    direction.deserialize(packet);
    uint32_t entType;
    packet >> entType;
    type = static_cast<Entity_Type>(entType);
  }
};
