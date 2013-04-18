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

void setUpMouse();

void setCamera(displaySet&);

void handleUserInput(displaySet&);

void movePlayer(displaySet&);

void turnPlayer(displaySet&);

} //end namespace gx
