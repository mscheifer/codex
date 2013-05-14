#pragma once
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
  bool render;
  v3_t position;
  v3_t acceleration;
  v3_t velocity;
  v3_t direction;
  Map* map;
  std::vector<BoundingObj*> boundingObjs;
  // Some kind of state {paralyzed, frozen, blah blah}
  // Power ups {contains MULTIPLERS for health, defense/ elemental weapons}

public:
  Entity() { render = true; }
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

  bool canRender() const { return render; }
  v3_t getPosition(void) const { return position; }
  void setPosition(v3_t c) { position = c;}
  v3_t getDirection(void) const { return direction; }
  void setDirection(v3_t d) { direction = d; }
  void setVelocity(v3_t v) { velocity = v; }
  void setMap(Map* m) { map = m; }
  void setBoundingObjs(std::vector<BoundingObj*> b){ boundingObjs = b; }
  std::vector<BoundingObj*> getBoundingObjs(){ return boundingObjs; }

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
