#include "Quadtree.h"

Quadtree::Quadtree(int pLevel, Rectangle pBounds):
level(pLevel),objects(),bounds(pBounds){
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
   nodes[0] = new Quadtree(level+1, Rectangle(BoundingObj::vec4_t(x+subWidth,y+subHeight,0),
     subWidth, subHeight));
   //Q2
   nodes[1] = new Quadtree(level+1, Rectangle(BoundingObj::vec4_t(x-subWidth,y+subHeight,0),
     subWidth, subHeight));
   //Q3
   nodes[2] = new Quadtree(level+1, Rectangle(BoundingObj::vec4_t(x-subWidth,y-subHeight,0),
     subWidth, subHeight));
   //Q4
   nodes[3] = new Quadtree(level+1, Rectangle(BoundingObj::vec4_t(x+subWidth,y-subHeight,0),
     subWidth, subHeight));
}

int Quadtree::getIndex(Rectangle& pRect) const{
  int index = -1;
  float minX = pRect.getCenter().x - pRect.getHalfWidth(); //lowest x
  float minY = pRect.getCenter().y - pRect.getHalfHeight(); //lowest y
  float maxX = pRect.getCenter().x + pRect.getHalfWidth(); //lowest x
  float maxY = pRect.getCenter().y + pRect.getHalfHeight(); //lowest y
  float midX = bounds.getCenter().x;// + (bounds.getWidth() / 2);
  float midY = bounds.getCenter().y;// + (bounds.getHeight() / 2);

  float maxBoundsX = bounds.getCenter().x + bounds.getHalfWidth();
  float minBoundsX = bounds.getCenter().x - bounds.getHalfWidth();
  float maxBoundsY = bounds.getCenter().y + bounds.getHalfHeight();
  float minBoundsY = bounds.getCenter().y - bounds.getHalfHeight();

  // Object can completely fit within the top quadrants
  bool topQuadrant = minY > midY && maxY < maxBoundsY;
    //&& y + pRect.getHalfHeight()*2 < horizontalMidpoint);
  /// Object can completely fit within the bottom quadrants
  bool bottomQuadrant = maxY < midY && minY > minBoundsY;
 
  //Q1 = 0
  //Q2 = 1
  //Q3 = 2
  //Q4 = 3
  // Object can completely fit within the left quadrants
  if ( maxX < midX && minX > minBoundsX) {
    if (topQuadrant)
      index = 1;
    else if (bottomQuadrant)
      index = 2;
  }
  // Object can completely fit within the right quadrants
  else if (minX > midX && maxX < maxBoundsX) {
    if (topQuadrant)
      index = 0;
    else if (bottomQuadrant)
      index = 3;
  }
 
  return index;
}

void Quadtree::insert(BoundingObj* o){
  if( o->getEntity() == nullptr )
    std::cout << "ERROR inserting Box with no entity associated with it" << std::endl;

  //very top most quadtree
  if(level == 0) {
    o->setQuadtree(this);
  }
  
  if (nodes[0] != nullptr) {
    int index = getIndex(*o->getRect());
 
    if (index != -1) {
      nodes[index]->insert(o);
      return;
    }
  }
 
  objects.push_front(o);
 
  //std::cout << "max Objs" << maxObjects << std::endl;
  //std::cout << "maxLevels" << maxLevels << std::endl;
  //split if you need to
  if (objects.size() > maxObjects && level < maxLevels) {
    if (nodes[0] == nullptr) {
        split();
        for(auto it = objects.begin(); it != objects.end(); ){
          int index = getIndex(*((*it)->getRect()));
          if( index != -1 ){
            nodes[index]->insert(*it);
            it = objects.erase(it);
          } else {
            it++;
          }
        }
    }
  }
}

void Quadtree::remove(BoundingObj* o) {
  int index = getIndex(*o->getRect());
  if( index != -1 && nodes[0] != nullptr) {
    nodes[index]->remove(o);
    return;
  } else {
    unsigned int beforeSize = objects.size(); 
    objects.remove(o);
    if( beforeSize == objects.size() )
    { /* std::cout << "I didn't remove anything " << std::endl;*/}
    else if(beforeSize -1 != objects.size() )
      std::cout << "error deleted more than one" << std::endl;
  }
}

std::vector<BoundingObj*> & Quadtree::retrieve(std::vector<BoundingObj*> & returnObjects, Rectangle* pRect){
  int index = getIndex(*pRect);
  //if(nodes[0]!=nullptr){
  if( index == -1 && nodes[0] != nullptr){
    nodes[0]->retrieve(returnObjects, pRect);
    nodes[1]->retrieve(returnObjects, pRect);
    nodes[2]->retrieve(returnObjects, pRect);
    nodes[3]->retrieve(returnObjects, pRect);
  }
  else if (index != -1 && nodes[0] != nullptr) {
    nodes[index]->retrieve(returnObjects, pRect);
  }
 
  for( auto it = objects.begin(); it != objects.end(); it++){
    if((*it)->getRect() != pRect)
      returnObjects.push_back(*it);
  }
  return returnObjects;
}

std::vector<BoundingObj*> & Quadtree::retrieve(std::vector<BoundingObj*> & returnObjects, BoundingObj* pRect){
  return retrieve(returnObjects, pRect->getRect());
}

//int Quadtree::size(){
//  if( nodes[0] == nullptr )
//    return objects.size();
//  else
//    return objects.size() + nodes[0]->size() + nodes[1]->size() + nodes[2]->size() + nodes[3]->size();
//}
//
//bool Quadtree::checkNullEntities(void* map){
//  if(nodes[0] == nullptr || nodes[1] == nullptr || nodes[2] == nullptr || nodes[3] == nullptr){
//    bool allNullptrs = nodes[0] == nullptr && nodes[1] == nullptr && nodes[2] == nullptr && nodes[3] == nullptr;
//    if(!allNullptrs){
//      std::cout << "ERROR quadtree should have all nullptrs or no null ptrs" << std::endl;
//      return false;
//    }
//  }
//
//  bool ret = true;
//  if(nodes[0] != nullptr){ //recursive calls
//    ret = ret && nodes[0]->checkNullEntities(map);
//    ret = ret && nodes[1]->checkNullEntities(map);
//    ret = ret && nodes[2]->checkNullEntities(map);
//    ret = ret && nodes[3]->checkNullEntities(map);
//  }
//  for( auto box = objects.begin(); box != objects.end(); box++ ){
//    if( (*box)->getEntity() == nullptr){
//      std::cout << "ERROR object on tree has entity nullptr" << std::endl;
//      ret = false;
//    }
//  }
//
//  return ret;
//}

//void Quadtree::test(){
//    Rectangle worldBounds(BoundingObj::vec4_t(0,0,0),1000,1000);
//    Quadtree q(0,worldBounds);
//    v3_t facingEast(1,0,0);
//    v3_t facingNorth(0,1,0);
//    v3_t up(0,0,1);
//    float width = ConfigManager::wallWidth();
//    float height = ConfigManager::wallHeight(); 
//    float depth = ConfigManager::wallDepth();
//
//    float wallX = 10;
//    float wallY = 8;
//
//    float centerX = 0;
//    float centerY = 0;
//    int i;
//    float startingX;
//    float startingY;
//    float startingXNeg;
//    float startingYNeg;
//    float startingZ = depth/2.0f;
//
//  for( i = 0,
//    startingX = ((wallX*width)/-2)+(width/2)+centerX,
//    startingY = ((wallY*width)/-2)+centerY,
//    startingYNeg = ((wallY*width)/2)+centerY;
//    i < wallX; i++, startingX += width )
//  {
//      std::cout << startingX << ", " << startingY << std::endl;
//      BoundingBox* bb = new BoundingBox(
//        BoundingBox(BoundingObj::vec4_t(startingX,startingY,startingZ),
//        BoundingObj::vec3_t(1,0,0),
//        BoundingObj::vec3_t(0,1,0),
//        BoundingObj::vec3_t(0,0,1),
//        width/2.f,height/2.f,depth/2.f));
//      bb->rotate(facingNorth, up);
//      //std::cout << "adding center " << bb->getCenter() << 
//      //  " hw " << bb->getHw() << " hh " << bb->getHh() << std::endl; 
//      q.remove(bb);
//      q.insert(bb);
//      
//      bb = new BoundingBox(
//        BoundingBox(BoundingObj::vec4_t(startingX,startingYNeg,startingZ),
//        BoundingObj::vec3_t(1,0,0),
//        BoundingObj::vec3_t(0,1,0),
//        BoundingObj::vec3_t(0,0,1),
//        width/2.f,height/2.f,depth/2.f));
//      bb->rotate(facingNorth, up);
//      //std::cout << "adding center " << bb->getCenter() << 
//      //  " hw " << bb->getHw() << " hh " << bb->getHh() << std::endl; 
//      q.remove(bb);
//      q.insert(bb);
//    }
//
//  for( i = 0,
//    startingX = ((wallX*width)/-2)+centerX,
//    startingXNeg = ((wallX*width)/2)+centerX,
//    startingY = ((wallY*width)/-2)+(width/2)+centerY;
//    i < wallY; i++, startingY += width )
//  {
//      std::cout << startingX << ", " << startingY << std::endl;
//
//      BoundingBox* bb = new BoundingBox(
//        BoundingBox(BoundingObj::vec4_t(startingX,startingY,startingZ),
//        BoundingObj::vec3_t(1,0,0),
//        BoundingObj::vec3_t(0,1,0),
//        BoundingObj::vec3_t(0,0,1),
//        width/2.f,height/2.f,depth/2.f));
//      bb->rotate(facingEast, up);
//      //std::cout << "adding center " << bb->getCenter() << 
//      //  " hw " << bb->getHw() << " hh " << bb->getHh() << std::endl; 
//      q.remove(bb);
//      q.insert(bb);
//      
//      bb = new BoundingBox(
//        BoundingBox(BoundingObj::vec4_t(startingX,startingYNeg,startingZ),
//        BoundingObj::vec3_t(1,0,0),
//        BoundingObj::vec3_t(0,1,0),
//        BoundingObj::vec3_t(0,0,1),
//        width/2.f,height/2.f,depth/2.f));
//      bb->rotate(facingEast, up);
//      //std::cout << "adding center " << bb->getCenter() << 
//      //  " hw " << bb->getHw() << " hh " << bb->getHh() << std::endl; 
//      q.remove(bb);
//      q.insert(bb);
//    }
//
//    BoundingBox* bb = new BoundingBox(
//      BoundingBox(BoundingObj::vec4_t(0,0,-5),
//      BoundingObj::vec3_t(1,0,0),
//      BoundingObj::vec3_t(0,1,0),
//      BoundingObj::vec3_t(0,0,1),
//      1000, 10, 1000));
//    q.remove(bb);
//    q.insert(bb);
//
//    BoundingBox* proj = new BoundingBox(
//      BoundingBox(BoundingObj::vec4_t(-183.15471f,-122.02121f,6.9363317f),
//      BoundingObj::vec3_t(1,0,0),
//      BoundingObj::vec3_t(0,1,0),
//      BoundingObj::vec3_t(0,0,1),
//      .5f, .5f, .5f));
//    q.remove(bb);
//    q.insert(bb);
//
//
//    std::vector<BoundingObj*> res;
//    res.clear();
//    q.retrieve(res, proj);
//    std::cout<< res.size() << "==1" << std::endl;
//
//    for(int i =0; i < res.size(); i++){
//      std::cout << res[i]->toString() << std::endl;
//    }
//
//  }
//


//GAP OF ROHAN TeST GOes in map.cpp
//with walls of size : wallx 10 wally 8
//// Wall, 
//wallWidth=37.4
//wallHeight=5
//wallDepth=10

 //   BoundingBox* projTest = new BoundingBox(
 //   BoundingBox(BoundingObj::vec4_t(182.00000f, 66.171684f, 5.0346622f),
 //   BoundingObj::vec3_t(1.0000000f, 0.00000000f, 0.00000000f),
 //   BoundingObj::vec3_t(0.00000000f, 1.0000000f, 0.00000000f),
 //   BoundingObj::vec3_t(0.00000000f, 0.00000000f, 1.0000000f),
 //   0.50000000f,0.50000000f,0.50000000f));

 //   BoundingBox* projTest2 = new BoundingBox(
 //   BoundingBox(BoundingObj::vec4_t(182.00000f, 66.171684f, 5.0346622f),
 //   BoundingObj::vec3_t(1.0000000f, 0.00000000f, 0.00000000f),
 //   BoundingObj::vec3_t(0.00000000f, 1.0000000f, 0.00000000f),
 //   BoundingObj::vec3_t(0.00000000f, 0.00000000f, 1.0000000f),
 //   0.50000000f,0.50000000f,0.50000000f));
 // 
 //   //q.insert(projTest);
 //   //q.remove(projTest);
 //   //q.insert(projTest);
 //   
 //   for(int i = 0 ; i < 9; i++)
 //     q.insert(projTest2);
 //   //q.remove(projTest2);

 // std::vector<BoundingObj*> res;
 // res.clear();
 // q.retrieve(res, projTest);

 // for( auto r = res.begin(); r!= res.end(); r++){
	//  float x = (*r)->getCenter().x;
	//  float y = (*r)->getCenter().y;
	//  float z = (*r)->getCenter().z;
	//if(  183 < x && x < 185 &&
	//	56 < y && y < 58 &&
	//	4 < z && z < 6 )
	//	std::cout << "------PASSTEST----" << std::endl;
 // }

 // if(res.size() == 0)
 //   std::cout << "-------fail-----" << std::endl;