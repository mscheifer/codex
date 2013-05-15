#ifndef ENTITYPOOL_H
#define ENTITYPOOL_H
#include <vector>
#include <assert.h>
#include "Wall.h"
#include "Projectile.h"
#include "Weapon.h"

class EntityPool {
    //don't modify main vectors for duration of serialization
	  std::vector<Wall>                 walls;
    std::vector<Projectile>           projectiles;
    std::vector<Weapon>               weapons;
    std::vector<PowerUp>              powerUps;

	  std::vector<Wall*>                newWalls;
    std::vector<Projectile*>          newProjectiles;
    std::vector<Weapon*>              newWeapons;
    std::vector<PowerUp*>             newPowerUps;

    std::vector<Wall>::iterator       wall_index;
    std::vector<Projectile>::iterator projectile_index;
    std::vector<Weapon>::iterator     weapon_index;
    std::vector<PowerUp>::iterator    powerUp_index;
  public:
    void reset() {
      walls.resize(walls.size() + newWalls.size());
      projectiles.resize(walls.size() + newProjectiles.size());
      weapons.resize(walls.size() + newWeapons.size());
      powerUps.resize(walls.size() + newPowerUps.size());

      newWalls.clear();
      newProjectiles.clear();
      newWeapons.clear();
      newPowerUps.clear();

      wall_index       = walls      .begin();
      projectile_index = projectiles.begin();
      weapon_index     = weapons    .begin();
      powerUp_index    = powerUps   .begin();
    }
    Entity* createEntity(sf::Uint32 type) {
      Entity* ret;
      switch (type) {
        case WALL:
          if(wall_index == walls.end()) {
            newWalls.push_back(new Wall());
            ret = *prev(newWalls.end());
          } else {
            ret = &*wall_index;
            wall_index++;
          }
          break;
        case PROJECTILE:
          if(projectile_index == projectiles.end()) {
            newProjectiles.push_back(new Projectile());
            ret = *prev(newProjectiles.end());
          } else {
            ret = &*projectile_index;
            projectile_index++;
          }
          break;
        case WEAPON:
          if(weapon_index == weapons.end()) {
            newWeapons.push_back(new Weapon());
            ret = *prev(newWeapons.end());
          } else {
            ret = &*weapon_index;
            weapon_index++;
          }
          break;
        case POWER_UP:
          if(powerUp_index == powerUps.end()) {
            newPowerUps.push_back(new PowerUp());
            ret = *prev(newPowerUps.end());
          } else {
            ret = &*powerUp_index;
            powerUp_index++;
          }
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
