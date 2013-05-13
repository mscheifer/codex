#ifndef ENTITYPOOL_H
#define ENTITYPOOL_H
#include <vector>
#include "Wall.h"
#include "Projectile.h"
#include "Weapon.h"

class EntityPool {
	  std::vector<Wall>                 walls;
    std::vector<Projectile>           projectiles;
    std::vector<Weapon>               weapons;

    std::vector<Wall>::iterator       wall_index;
    std::vector<Projectile>::iterator projectile_index;
    std::vector<Weapon>::iterator     weapon_index;
  public:
    void reset() {
      wall_index       = walls.begin();
      projectile_index = projectiles.begin();
      weapon_index     = weapons.begin();
    }
    Entity* createEntity(uint32_t type) {
      Entity* ret;
      switch (type) {
        case WALL:
          if(wall_index == walls.end()) {
            walls.push_back(Wall());
            wall_index = --walls.end();
          }
          ret = &*wall_index;
          wall_index++;
          break;
        case PROJECTILE:
          if(projectile_index == projectiles.end()) {
            projectiles.push_back(Projectile());
            projectile_index = --projectiles.end();
          }
          ret = &*projectile_index;
          projectile_index++;
          break;
        case WEAPON:
          if(weapon_index == weapons.end()) {
            weapons.push_back(Weapon());
            weapon_index = --weapons.end();
          }
          ret = &*weapon_index;
          weapon_index++;
          break;
      }
      return ret;
    }
};
#endif