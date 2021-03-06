#include "Entity.h"
#include <algorithm>
#include <iostream>

//@mc 2box
std::vector<std::pair<Entity*,BoundingObj::vec3_t>> Entity::detectCollision(){
  std::vector<std::pair<Entity*,BoundingObj::vec3_t>> res;
  std::vector<BoundingObj*> potentialCollisions;
  potentialCollisions.clear();
  res.clear();

  //all of my bounding objs
  std::vector<BoundingObj*> vec = getBoundingObjs();
  for(auto myObjsIt = vec.begin(); myObjsIt != vec.end(); myObjsIt++){
    map->getQuadtreePtr()->retrieve(potentialCollisions, *myObjsIt);

    //all potential collisions with bounding obj[i]
    for(auto it2 = potentialCollisions.begin(); it2 != potentialCollisions.end(); it2++){
      
      //don't collide with self
      if((*it2)->getEntity() == this)
        continue;

      std::pair<bool,BoundingObj::vec3_t> collRes = collide(*myObjsIt,*it2);
      //try collide
      if(collRes.first){
        auto result = res.begin();

      //check if I have already collided with this entity
      //[for 2 objs collide with 1 of my bboxes] shortest dist changes
        for( ; result != res.end(); result++){
          if( result->first == (*it2)->getEntity() ){
            //pick max if same entity has collided
            if(result->second.magnitudesq() < collRes.second.magnitudesq()){
              res.erase(result);
              res.push_back(std::pair<Entity*,BoundingObj::vec3_t>((*it2)->getEntity(),collRes.second));
            }
            break;
          }
        }

        if(result == res.end()){
          res.push_back(std::pair<Entity*,BoundingObj::vec3_t>((*it2)->getEntity(),collRes.second));
        }

      }

    }
  }

  //sort
  std::sort (res.begin(), res.end(), sortEntity);

  return res;
}
  void Entity::serialize(sf::Packet& packet) const
  {
    position.serialize(packet);
    direction.serialize(packet);
  }

  void Entity::deserialize(sf::Packet& packet)
  {    
    position.deserialize(packet);
    direction.deserialize(packet);
  }

std::vector<RayCollision> Entity::detectCollision(Ray* r){
  std::vector<RayCollision> res;
  std::vector<BoundingObj*> potentialCollisions;
  potentialCollisions.clear();
  res.clear();

  //all of my bounding objs
  map->getQuadtreePtr()->retrieve(potentialCollisions, r);

  //all potential collisions with bounding obj[i]
  for(auto it2 = potentialCollisions.begin(); it2 != potentialCollisions.end(); it2++){
      
    //don't collide with self
    if((*it2)->getEntity() == this)
      continue;

    //check if I have already collided with this entity //TODO maybe no entity check 
    //[for 2 objs collide with 1 of my bboxes] shortest dist changes
    auto finder = res.begin();
    bool flag = false;
    for( ; finder != res.end(); finder++){
      if( finder->e == (*it2)->getEntity() ){
        flag = true;
        break;
      }
    }
    if(flag)
      continue;

    if( finder == res.end() ){
      RayCollision collRes = rayCollide(r,*it2);
      //try collide
      if(collRes.collided){
        collRes.e = (*it2)->getEntity();
        res.push_back(collRes);
      }
    }
  }

  //sort by greater because a longer hit will mean it was hit first
  std::sort (res.begin(), res.end(), sortRayCollision);

  return res;
}

v3_t Entity::correctMovement(v3_t movementDirection, bool slide, v3_t origin){
  BoundingBox * myBox = (BoundingBox*) boundingObjs[0]; //TODO just doing this for now
  //add the radius to account for collision
  //v3_t radius = myBox->getMaxRadius( movementDirection );
  //movementDirection += radius;
  Ray movementRay(v4_t(position.x,position.y,position.z), movementDirection);//TODO @mc ray movement adjust origin
  
  std::vector<RayCollision> colls = detectCollision(&movementRay);
  bool restart = false;
  int restarts = 0;

  //std::cout << movementRay.getDirection() << std::endl;
  for(auto coll = colls.begin(); coll != colls.end(); ){

    if(correctMovementHit(coll->e)){
      //scale by tfirst
      v3_t newDir = movementRay.getDirection();
      newDir.scale(coll->tfirst);
        
      if(slide){
        //project max "radius" onto normal and add the largest
        //adjust normal axis to be in opposite direction as movement (pi/2 - -pi/2)
        if( newDir.dot(coll->normalAxis) > 0 ){
          coll->normalAxis.negate();
        }
        coll->normalAxis.normalize();

        //get the max "radius" on teh normal (sliding out)
        coll->normalAxis = myBox->getMaxRadius(coll->normalAxis);
        //subtract the max radius (normal axis is in opposite direction)
        newDir += coll->normalAxis;

        //project extra onto axis parallel and add that (the slide)
        v3_t excess = movementRay.getDirection();
        excess.scale(1.0f - coll->tfirst);
        coll->parallelAxis.normalize();
        length_t slideLen = excess.dot(coll->parallelAxis);
        excess = coll->parallelAxis;
        excess.scale(slideLen);
        newDir += excess;
      }

      movementRay.setDirection(newDir);
      restart = true;
    }

    if(restart){
      restarts++;
      restart = false;
      colls = detectCollision(&movementRay);
      coll = colls.begin();
    } else {
      coll++;
    }

    if( restarts > 3 ) break;
  }
  return movementRay.getDirection();
}

void Entity::removeFromMap(){
  map->removeFromQtree(this);
  //render = false;
}

Entity::~Entity(){
  for(auto boxes = boundingObjs.begin(); boxes != boundingObjs.end(); boxes++){
    delete *boxes;
  }
}