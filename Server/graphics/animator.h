#include <array>
#include <utility>

class Player;

namespace gx {

struct animator {
    static const double animationLength;
    animator();
    //TODO: return two frames from two animations for interpolation
    typedef std::pair<unsigned int,double> animationKey_t;
    animationKey_t updatePlayer(Player const&); //returns their current animation and frame
  private:
    std::array<animationKey_t,4> currentAnimations; //index based on player id 
};
} //end namespace gx