#include "animator.h"
#include "Player.h"

const double gx::animator::animationLength = 100.0;

gx::animator::animator() {
  for(auto itr = this->currentAnimations.begin(); 
           itr != this->currentAnimations.end(); itr++) {
    itr->first = 0;
    itr->second = 0.0;
  }
}

gx::animator::animationKey_t gx::animator::updatePlayer(Player const& p) {
  auto& anim = this->currentAnimations[p.player_id];
  unsigned int type = anim.first;
  if(!(type == 2 || type == 3) || anim.second >= 98.0) {
    type = 0;
    if(p.charging) type = 4;
    if(p.walking) type = 1;
    if(p.meleeAttack) type = 2;
    if(p.shotProjectile) type = 3;
  }
  anim.first = type;
  anim.second++;
  anim.second = std::fmod(anim.second,animationLength);
  return anim;
}