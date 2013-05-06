#include "userInput.h"
#include "vector3.h"
#include "displaySet.h"

namespace {
const double mouseSensitivity = 0.001;

gx::vector3  mouseDirection;
sf::Vector2i mouseBasePosition;
sf::Vector2i mouseDiff;

} //end unnamed namespace

gx::userInput::userInput(move_t mv, vector3 d, bool jump, bool close, bool f)
  : move(mv), dir(d), jumped(jump), stopped(close), fire(f) {}

move_t gx::userInput::getMove() {
  return this->move;
}
gx::vector3 gx::userInput::getDir() {
  return this->dir;
}
bool gx::userInput::getJump() {
  return this->jumped;
}
bool gx::userInput::getStop() {
  return this->stopped;
}
  bool gx::userInput::getFire() {
  return this->fire;
}

void gx::setUpMouse() {
  //somehow change this to set it to the center of the screen
  mouseBasePosition = sf::Vector2i(200,200);
  sf::Mouse::setPosition(mouseBasePosition);
}

move_t gx::movePlayer() {
  move_t movement = NULL_DIR;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      movement = FORWARD_LEFT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      movement = FORWARD_RIGHT;
    } else {
      movement = FORWARD;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      movement = BACKWARD_LEFT;
    } else {
      movement = LEFT;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      movement = BACKWARD_RIGHT;
    } else {
      movement = RIGHT;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    movement = BACKWARD;
  }
  return movement;
}

gx::vector3 gx::turnPlayer() {
  //base direction needs to be looking forward because mouse movement needs to rotate
  //around the axis that are 90 degrees away from the base direction and its
  //easier if these are the x and z axis
  const vector3 basePlayerDirection = gx::vector3( 0.0, 1.0, 0.0);

  sf::Vector2i curPosition = sf::Mouse::getPosition();
  if(curPosition != mouseBasePosition) {
    sf::Vector2i newDiff = curPosition - mouseBasePosition;
    mouseDiff += newDiff;
    mouseDirection = rotateZ(-mouseDiff.x * mouseSensitivity) * 
          rotateX( mouseDiff.y * mouseSensitivity) * basePlayerDirection;

    sf::Mouse::setPosition(mouseBasePosition);
  }
  return mouseDirection;
}
