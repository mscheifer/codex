#include "userInput.h"
#include "vector3.h"
#include "displaySet.h"

namespace {
//player needs to start looking forward because mouse movement needs to rotate
//around the axis that are 90 degrees away from the starting direction and its
//easier if these are the x and y axis
gx::vector3 startPlayerDirection = gx::vector3( 0.0, -1.0, 0.0);

gx::vector3 playerPosition  = gx::vector3( 0.0,  0.0,  0.0);
gx::vector3 playerDirection = startPlayerDirection;
gx::vector3 upDirection     = gx::vector3( 0.0,  0.0,  1.0);

const double mouseSensitivity = 0.001;

sf::Vector2i mouseBasePosition;
sf::Vector2i mouseDiff;

} //end unnamed namespace

gx::userInput::userInput(move_t mv, vector3 d, bool jump, bool close, bool fire)
  : move(mv), dir(d), jumped(jump), stopped(close) {}

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

void gx::setCamera(displaySet& display) {
  //add the direction vector to the player's position to get the position to
  //look at
  display.setView(playerPosition,
                  playerDirection + playerPosition,
                  upDirection);
}

move_t gx::movePlayer(displaySet& display) {
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
  if(movement != NULL_DIR) {
    matrix rotation = rotateZ(movementAngles[movement]);
    vector3 diff = rotation * vector3(playerDirection[0],playerDirection[1],0);
    diff.normalize();
    diff.scale(0.1f);
    playerPosition += diff;

    setCamera(display);
  }
  return movement;
}

gx::vector3 gx::turnPlayer(displaySet& display) {
  sf::Vector2i curPosition = sf::Mouse::getPosition();
  if(curPosition != mouseBasePosition) {
    sf::Vector2i newDiff = curPosition - mouseBasePosition;
    mouseDiff += newDiff;
    playerDirection = rotateZ(-mouseDiff.x * mouseSensitivity) * 
      rotateX(mouseDiff.y * mouseSensitivity) * startPlayerDirection;

    sf::Mouse::setPosition(mouseBasePosition);

    setCamera(display);
  }
  return playerDirection;
}
