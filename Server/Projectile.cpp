#include "Projectile.h"


Projectile::Projectile(void)
{
}


Projectile::~Projectile(void)
{
}

void Projectile::setOwner(Player * player)
{
  owner = player;
}