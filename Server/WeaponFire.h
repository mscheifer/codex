#pragma once
#include "Weapon.h"

class WeaponFire :
	public Weapon
{
public:
  static const bool hasRangedAttack = true;
  static const bool hasMeleeAttack = true;

	WeaponFire(v3_t c, Map *, MAGIC_POWER basicAttack1);
  void updateBounds();
  Projectile* attackRange(v3_t dir , v3_t pos, Player* owner);

  WeaponType getWeaponType() const;
  bool getHasRangedAttack() const { return hasRangedAttack; }
  bool getHasMeleeAttack() const { return hasMeleeAttack; }
  float getMpCost();
};

