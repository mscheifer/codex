#pragma once
#include <vector>
#include "Map.h"
#include "boundingManager.h"
#include "StaticEnums.h"
#include "graphics/matrix.h"

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
  v3_t correctMovement( v3_t movementDirection, bool slide );
  virtual bool correctMovementHit( Entity* ){return false;}

public:
  Entity() { render = true; }
  virtual ~Entity() {}
  
  virtual void update(){}
  virtual void handleCollisions(){}
  virtual void clearEvents(){}
  
  //update the bounding objects
  //the bounding objects should automatically update thier position on the tree
  virtual void updateBounds(){}
  //this is only for showing collision boxes on client, should never be called
  virtual void updateBoundsSoft(){}
  //return a vector of entites collided with and shortest vector of removal
  std::vector<std::pair<Entity*,v3_t>> detectCollision();

  //movement collision functions
  std::vector<RayCollision> detectCollision(Ray* r);



  bool canRender() const { return render; }
  v3_t getPosition(void) const { return position; }
  void setPosition(v3_t c) { position = c; }
  v3_t getDirection(void) const { return direction; }
  void setDirection(v3_t d) { direction = d; }
  void setVelocity(v3_t v) { velocity = v; }
  void setMap(Map* m) { map = m; }
  void setBoundingObjs(std::vector<BoundingObj*> b){ boundingObjs = b; }
  std::vector<BoundingObj*> getBoundingObjs(){ return boundingObjs; }

  virtual void serialize(sf::Packet& packet) const;
  virtual void deserialize(sf::Packet& packet);
 
  virtual Entity_Type getType() const { return type; }
};
