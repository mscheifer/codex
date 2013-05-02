#include "Quadtree.h"

Quadtree::Quadtree(int pLevel, Rectangle pBounds):
level(pLevel),bounds(pBounds){
  for( unsigned int i = 0; i < nodes.size(); i++){
    nodes[i] = nullptr;
  }
}

Quadtree::~Quadtree(){
  for( unsigned int i = 0; i < nodes.size(); i++){
    if( nodes[i] != nullptr )
      delete nodes[i];
  }
}

void Quadtree::clear(){
  for( unsigned int i = 0; i < nodes.size(); i++){
    if(nodes[i] != nullptr){
      nodes[i]->clear();
      nodes[i] = nullptr;
    }
  }
  objects.clear();
}

void Quadtree::split(){
   float subWidth = (bounds.getHalfWidth() / 2);
   float subHeight = (bounds.getHalfHeight() / 2);
   float x = bounds.getCenter().x;
   float y = bounds.getCenter().y;
   //Q1
   nodes[0] = new Quadtree(level+1, Rectangle(gx::vector4(x+subWidth,y-subHeight,0),
     subWidth, subHeight));
   //Q2
   nodes[1] = new Quadtree(level+1, Rectangle(gx::vector4(x-subWidth,y-subHeight,0),
     subWidth, subHeight));
   //Q3
   nodes[2] = new Quadtree(level+1, Rectangle(gx::vector4(x-subWidth,y+subHeight,0),
     subWidth, subHeight));
   //Q4
   nodes[3] = new Quadtree(level+1, Rectangle(gx::vector4(x+subWidth,y+subHeight,0),
     subWidth, subHeight));
}

//TODO clarify the x y z
int Quadtree::getIndex(BoundingObj o){
  Rectangle pRect = *o.getRect();
  int index = -1;
  float x = pRect.getCenter().x - pRect.getHalfWidth(); //lowest x
  float y = pRect.getCenter().y - pRect.getHalfHeight(); //lowest y
  float verticalMidpoint = bounds.getCenter().x;// + (bounds.getWidth() / 2);
  float horizontalMidpoint = bounds.getCenter().y;// + (bounds.getHeight() / 2);

  // Object can completely fit within the top quadrants
  bool topQuadrant = (y < horizontalMidpoint 
    && y + pRect.getHalfHeight()*2 < horizontalMidpoint);
  // Object can completely fit within the bottom quadrants
  bool bottomQuadrant = (y > horizontalMidpoint);
 
  // Object can completely fit within the left quadrants
  if (x < verticalMidpoint && x + pRect.getHalfWidth()*2 < verticalMidpoint) {
    if (topQuadrant) {
      index = 1;
    }
    else if (bottomQuadrant) {
      index = 2;
    }
  }
  // Object can completely fit within the right quadrants
  else if (x > verticalMidpoint) {
    if (topQuadrant) {
      index = 0;
    }
    else if (bottomQuadrant) {
      index = 3;
    }
  }
 
  return index;
}

void Quadtree::insert(BoundingObj& o){
  Rectangle pRect = *o.getRect();
  if(level == 0)
    o.setQuadtree(this);
  if (nodes[0] != nullptr) {
    int index = getIndex(o);
 
    if (index != -1) {
      nodes[index]->insert(o);
      return;
    }
  }
 
  objects.push_front(o);
 
  if (objects.size() > maxObjects && level < maxLevels) {
    if (nodes[0] == nullptr) {
        split();
    }
 
    unsigned int i = 0;

    for( std::list<BoundingObj>::iterator it = objects.begin(); it != objects.end(); ){
      int index = getIndex(*it);
      if( index != -1 ){
        nodes[index]->insert(*it);
        it = objects.erase(it);
      }
      else
        it++;
    }
  }
}

void Quadtree::remove(BoundingObj& o){
  int index = getIndex(o);
  if( index != -1 && nodes[0] != nullptr){
    nodes[index]->remove(o);
    return;
  }
  else
    objects.remove(o);
}

std::list<BoundingObj> & Quadtree::retrieve(std::list<BoundingObj> & returnObjects, BoundingObj pRect){
  int index = getIndex(pRect);
  if( index == -1 && nodes[0] != nullptr){
    nodes[0]->retrieve(returnObjects, pRect);
    nodes[1]->retrieve(returnObjects, pRect);
    nodes[2]->retrieve(returnObjects, pRect);
    nodes[3]->retrieve(returnObjects, pRect);
  }
  else if (index != -1 && nodes[0] != nullptr) {
    nodes[index]->retrieve(returnObjects, pRect);
  }
 
  for( std::list<BoundingObj>::iterator it = objects.begin(); it != objects.end(); it++){
    if(*it != pRect)
      returnObjects.push_front(*it);
  }
  return returnObjects;
}