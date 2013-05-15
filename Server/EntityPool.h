#ifndef ENTITYPOOL_H
#define ENTITYPOOL_H
#include <vector>
#include <memory>
#include <assert.h>
#include "Wall.h"
#include "Projectile.h"
#include "Weapon.h"

template<typename T>
class objPool {
    //don't modify main vector for duration of serialization
	           std::vector<T>                  objs;
	           std::vector<std::unique_ptr<T>> newObjs;
    typename std::vector<T>::iterator        objsIndex;
  public:
    void reset() {
      objs.resize(objs.size() + newObjs.size());
      newObjs.clear();
      objsIndex = objs.begin();
    }
    T* createEntity() {
      T* ret;
      if(objsIndex == objs.end()) {
        newObjs.push_back(std::unique_ptr<T>(new T()));
        ret = prev(newObjs.end())->get();
      } else {
        ret = &*objsIndex;
        objsIndex++;
      }
      return ret;
    }
};

class EntityPool {
	  objPool<Wall>                 walls;
    objPool<Projectile>           projectiles;
    objPool<Weapon>               weapons;
    objPool<PowerUp>              powerUps;
  public:
    void reset() {
      walls.reset();
      projectiles.reset();
      weapons.reset();
      powerUps.reset();
    }
    Entity* createEntity(sf::Uint32 type) {
      Entity* ret;
      switch (type) {
        case WALL:
          ret = walls.createEntity();
          break;
        case PROJECTILE:
          ret = projectiles.createEntity();
          break;
        case WEAPON:
          ret = weapons.createEntity();
          break;
        case POWER_UP:
          ret = powerUps.createEntity();
          break;
        case PLAYER:
          assert(false);
          break;
        default:
          ret = nullptr;
      }
      return ret;
    }
};
#endif
