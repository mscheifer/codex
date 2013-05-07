#include "boundingObj.h"
#include "Quadtree.h"

void BoundingObj::updateOnTree(){ 
  getQtree()->remove(this);
  updateRect();
  getQtree()->insert(this);
}

BoundingObj::~BoundingObj(){
  if(qtree)
    qtree->remove(this);
  if(rect)
    delete rect;
}