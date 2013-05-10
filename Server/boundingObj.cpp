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