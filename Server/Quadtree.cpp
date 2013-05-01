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
   double subWidth = (bounds.getWidth() / 2);
   double subHeight = (bounds.getHeight() / 2);
   double x = bounds.getX();
   double y = bounds.getY();
   nodes[0] = new Quadtree(level+1, Rectangle(x + subWidth, y, subWidth, subHeight));
   nodes[1] = new Quadtree(level+1, Rectangle(x, y, subWidth, subHeight));
   nodes[2] = new Quadtree(level+1, Rectangle(x, y + subHeight, subWidth, subHeight));
   nodes[3] = new Quadtree(level+1, Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));
}

//TODO clarify the x y z
int Quadtree::getIndex(Rectangle pRect){
  int index = -1;
  double verticalMidpoint = bounds.getX() + (bounds.getWidth() / 2);
  double horizontalMidpoint = bounds.getY() + (bounds.getHeight() / 2);

  // Object can completely fit within the top quadrants
  bool topQuadrant = (pRect.getY() < horizontalMidpoint && pRect.getY() + pRect.getHeight() < horizontalMidpoint);
  // Object can completely fit within the bottom quadrants
  bool bottomQuadrant = (pRect.getY() > horizontalMidpoint);
 
  // Object can completely fit within the left quadrants
  if (pRect.getX() < verticalMidpoint && pRect.getX() + pRect.getWidth() < verticalMidpoint) {
    if (topQuadrant) {
      index = 1;
    }
    else if (bottomQuadrant) {
      index = 2;
    }
  }
  // Object can completely fit within the right quadrants
  else if (pRect.getX() > verticalMidpoint) {
    if (topQuadrant) {
      index = 0;
    }
    else if (bottomQuadrant) {
      index = 3;
    }
  }
 
  return index;
}

void Quadtree::insert(Rectangle pRect){
  if (nodes[0] != nullptr) {
    int index = getIndex(pRect);
 
    if (index != -1) {
      nodes[index]->insert(pRect);
      return;
    }
  }
 
  objects.push_front(pRect);
 
  if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
    if (nodes[0] == nullptr) {
        split();
    }
 
    unsigned int i = 0;
    while (i < objects.size()) {
      int index = getIndex(objects.front());
      if (index != -1) {
        nodes[index]->insert(objects.front());
        objects.pop_front();
      }
      else {
        i++;
      }
    }  
  }
}

void Quadtree::remove(Rectangle pRect){
  if(nodes[0] != nullptr){
      int index = getIndex(pRect);
      if( index != -1 ){
          nodes[index]->remove(pRect);
          return;
      }
  }
  objects.remove(pRect);
}

std::list<Rectangle> & Quadtree::retrieve(std::list<Rectangle> & returnObjects, Rectangle pRect){
  int index = getIndex(pRect);
  if( index == -1 && nodes[0] != nullptr){
    nodes[0]->retrieve(returnObjects, pRect);
    nodes[1]->retrieve(returnObjects, pRect);
    nodes[2]->retrieve(returnObjects, pRect);
    nodes[3]->retrieve(returnObjects, pRect);
  }
  if (index != -1 && nodes[0] != nullptr) {
    nodes[index]->retrieve(returnObjects, pRect);
  }
 
  for( std::list<Rectangle>::iterator it = objects.begin(); it != objects.end(); it++){
    returnObjects.push_front(*it);
  }
  return returnObjects;
}