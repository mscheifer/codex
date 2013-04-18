#include "userInput.h"

namespace {
gx::vector3 startPlayerDirection = gx::vector3(-4.0, -2.0, -1.0);

gx::vector3 playerPosition  = gx::vector3( 5.0,  2.0,  5.0);
gx::vector3 playerDirection = startPlayerDirection;
gx::vector3 upDirection     = gx::vector3( 0.0,  1.0,  0.0);

const double mouseSensitivity = 0.01;

sf::Vector2i startMousePosition;
sf::Vector2i prevMousePosition;

const double movementAngles[8] = {
  0.0,
  M_PI_4,
  M_PI_2,
  M_PI_2 + M_PI_4,
  M_PI,
  -(M_PI_2 + M_PI_4),
  -M_PI_2,
  -M_PI_4,
};

} //end unnamed namespace

void gx::setUpMouse() {
  //somehow change this to set it to the center of the screen
  sf::Mouse::setPosition(sf::Vector2i(200,200));
  startMousePosition = sf::Mouse::getPosition();
  prevMousePosition = sf::Mouse::getPosition();
}

void gx::setCamera(displaySet& display) {
  display.setView(playerPosition,
                  playerDirection,
                  upDirection);
}

void gx::handleUserInput(displaySet& display) {
  movePlayer(display);
  turnPlayer(display);
}

void gx::movePlayer(displaySet& display) {
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
    matrix rotation = rotateY(movementAngles[movement]);
    vector3 diff = rotation * vector3(playerDirection[0],0,playerDirection[2]);
    diff.normalize();
    diff.scale(0.1);
    playerPosition += diff;

    setCamera(display);
  }
}

void gx::turnPlayer(displaySet& display) {
  sf::Vector2i curPosition = sf::Mouse::getPosition();
  if(curPosition != prevMousePosition) {
    prevMousePosition == curPosition;
    sf::Vector2i diff = curPosition - startMousePosition;
    vector3 newDirection = rotateY(diff.x * mouseSensitivity) * 
      rotateX(diff.y * mouseSensitivity) * startPlayerDirection;
    playerDirection = newDirection;

    setCamera(display);
  }
}
