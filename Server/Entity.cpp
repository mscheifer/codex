#include "Entity.h"

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

      //check if I have already collided with this entity //TODO maybe no entity check 
      //[for 2 objs collide with 1 of my bboxes] shortest dist changes
      auto finder = res.begin();
      bool flag = false;
      for( ; finder != res.end(); finder++){
        if( finder->first == (*it2)->getEntity() ){
          flag = true;
          break;
        }
      }
      if(flag)
        continue;

      if( finder == res.end() ){
        std::pair<bool,BoundingObj::vec3_t> collRes = collide(*myObjsIt,*it2);
        //try collide
        if(collRes.first){
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
      ConfigManager::log( r->toString() );
      ConfigManager::log( (*it2)->toString() );
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

v3_t Entity::correctMovement(v3_t movementDirection, bool slide){
  BoundingBox * myBox = (BoundingBox*) boundingObjs[0];
  //add the radius to account for collision
  //v3_t radius = myBox->getMaxRadius( movementDirection );
  //movementDirection += radius;
  Ray movementRay(v4_t(position.x,position.y,position.z), movementDirection);
  
  std::vector<RayCollision> colls = detectCollision(&movementRay);
  bool restart = false;
  int restarts = 0;

  for(auto coll = colls.begin(); coll != colls.end(); ){
    Entity * e = coll->e;
    v3_t acceptedMove = movementRay.getDirection();

    if(correctMovementHit(coll->e)){
        //scale by tfirst
        v3_t newDir = acceptedMove;
        newDir.scale(coll->tfirst);
        
        if(slide){
          //project max "radius" onto normal and add the largest
          //adjust normal axis to be in opposite direction as movement (pi/2 - -pi/2)
          if( newDir.dot(coll->normalAxis) > 0 ){
            coll->normalAxis.negate();
          }
          coll->normalAxis.normalize();

          //get the max "radius" on teh normal
          //TODO just doing this for now
          coll->normalAxis = myBox->getMaxRadius(coll->normalAxis);

          //subtract the max radius (normal axis is in opposite direction)
          newDir += coll->normalAxis;

          //project extra onto axis parallel and add that
          v3_t excess = acceptedMove;
          excess.scale(1.0f - coll->tfirst);
          coll->parallelAxis.normalize();
          length_t slide = excess.dot(coll->parallelAxis);
          excess = coll->parallelAxis;
          excess.scale(slide);
          newDir += excess;
        }

        movementRay.setDirection(newDir);
        restart = true;
        break;
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
