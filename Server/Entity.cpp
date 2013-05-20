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
        break;

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
      break;

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