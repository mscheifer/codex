#include "PowerUpHealthPack.h"


PowerUpHealthPack::PowerUpHealthPack(v3_t p, Map* m):PowerUp(p , m, NONE)
{
  healthMutiplyer = 100;

}


PowerUpHealthPack::~PowerUpHealthPack(void)
{
}
