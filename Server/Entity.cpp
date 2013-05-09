#include "Entity.h"

std::vector<std::pair<Entity*,BoundingObj::vec3_t>> Entity::detectCollision(){
  std::vector<std::pair<Entity*,BoundingObj::vec3_t>> res;
  std::vector<BoundingObj*> potentialCollisions;
  potentialCollisions.clear();
  res.clear();

  //all of my bounding objs
  std::vector<BoundingObj*> vec = getBoundingObjs();
  for(auto it = vec.begin(); it != vec.end(); it++){
    map->getQuadtreePtr()->retrieve(potentialCollisions, *it);

    //all potential collisions with bounding obj[i]
    for(auto it2 = potentialCollisions.begin(); it2 != potentialCollisions.end(); it2++){
      
      //check if entity is inside//TODO maybe no entity check 
      //[for 2 objs collide with 1 of my bboxes] shortest dist changes
      auto finder = res.begin();
      for( ; finder != res.end(); finder++){
        if( finder->first == (*it)->getEntity() )
          break;
      }
      if( finder == res.end() ){
        std::pair<bool,BoundingObj::vec3_t> collRes = collide(*it,*it2);
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