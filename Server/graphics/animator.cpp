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
  this->currentAnimations[p.player_id].second++;
  this->currentAnimations[p.player_id].second = 
    std::fmod(this->currentAnimations[p.player_id].second,animationLength);
  return this->currentAnimations[p.player_id];
}