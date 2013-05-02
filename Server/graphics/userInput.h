#include <SFML/Window.hpp>
#include "StaticEnums.h"
#include "vector3.h"

namespace gx {

class displaySet;

class userInput {
    move_t move;
    vector3 dir;
    bool   jumped;
    bool   stopped;
  public:
    userInput(move_t,vector3,bool,bool);
    move_t getMove();
    vector3 getDir();
    bool getJump();
    bool getStop();
};

void setUpMouse();

void setCamera(displaySet&);

move_t movePlayer(displaySet&);

vector3 turnPlayer(displaySet&);

} //end namespace gx
