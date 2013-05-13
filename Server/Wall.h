#pragma once
#include "Entity.h"
#include <vector>
class Wall :
  public Entity
{
public:
  static const float sphereRadius;
  static const Entity_Type type = WALL;
  Wall(unsigned int width, unsigned int depth, unsigned int height, v3_t startingCoordinate, v3_t direct, Map*);
  ~Wall(void);
  void update(void);
  void addNewCenter(v3_t);
  void addNewCenters(std::vector<v3_t>&);
  void updateBounds();
  void updateBoundsSoft(); 
  Entity_Type getType() {
    return type;
  }
private:
  // A vector of POSSIBLE positions
  std::vector<v3_t> centerPositions;
  unsigned int currentCenter;
  unsigned int width;
  unsigned int depth;
  unsigned int height;
};

