#include <SFML/Window.hpp>
#include "StaticEnums.h"
#include "vector3.h"

namespace gx {

class userInput {
    move_t move;
    vector3 dir;
    bool   jumped;
    bool   stopped;
	bool   fire;
  public:
    userInput(move_t,vector3,bool,bool,bool);
    move_t getMove();
    vector3 getDir();
    bool getJump();
    bool getStop();
	bool getFire();
};

void setUpMouse();

move_t movePlayer();

vector3 turnPlayer();

} //end namespace gx
