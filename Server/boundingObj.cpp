#include "boundingObj.h"
#include "Quadtree.h"

void BoundingObj::updateOnTree(){ 
  qtree->remove(this);
  updateRect();
  qtree->insert(this);
}

BoundingObj::~BoundingObj(){
  if(qtree)
    qtree->remove(this);
  if(rect)
    delete rect;
}

std::string BoundingObj::toString(){
  std::stringstream ss;
 // ss << "rectangle " << std::endl
 //   << "center " << rect->getCenter() << " w " << rect->getHalfWidth() << " h " << 
 //   rect->getHalfHeight() << std::endl 
 //   << " object " << std::endl
  ss << " center " << center << std::endl;
  return ss.str();
}