#include "PowerUpSpeedPack.h"


PowerUpSpeedPack::PowerUpSpeedPack(void)
{
}

PowerUpSpeedPack::PowerUpSpeedPack(v3_t p, Map* m):PowerUp(p , m, NONE)
{
  speedMutiplyer = 5.0;
}


PowerUpSpeedPack::~PowerUpSpeedPack(void)
{
}
