#include <SFML/Window.hpp>
#include "displaySet.h"

namespace gx {

enum move_t {
  FORWARD = 0,
  FORWARD_LEFT,
  LEFT,
  BACKWARD_LEFT,
  BACKWARD,
  BACKWARD_RIGHT,
  RIGHT,
  FORWARD_RIGHT,
  NULL_DIR
};

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
