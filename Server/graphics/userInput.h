#include <SFML/Window.hpp>
#include "StaticEnums.h"

namespace gx {

class displaySet;

struct userInput {
  move_t move;
  //direction
  bool   jumped;
  bool   stopped;
};

void setUpMouse();

void setCamera(displaySet&);

move_t movePlayer(displaySet&);

void turnPlayer(displaySet&);

} //end namespace gx
