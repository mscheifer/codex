#include "input.h"
#include <SFML/Window.hpp>
#include "matrix.h"

namespace {
const double mouseSensitivity = 0.001;

//base direction needs to be looking forward because mouse movement needs to rotate
//around the axis that are 90 degrees away from the base direction and its
//easier if these are the x and z axis
const gx::vector3f basePlayerDirection = gx::vector3f( 0.0, 1.0, 0.0);
 
gx::vector3f mouseDirection = basePlayerDirection;
sf::Vector2i mouseBasePosition;
sf::Vector2i mouseDiff = sf::Vector2i(0,0);

} //end unnamed namespace

gx::input::input()
  : updated(false), jumped(false), stopped(false), fired1(false), fired2(false), pickup(false),switchWeapon(false) {}

bool gx::input::getUpdated() {
  return this->updated;
}
bool gx::input::getJump() {
  return this->jumped;
}
bool gx::input::getStop() {
  return this->stopped;
}
bool gx::input::fire1() {
  return this->fired1;
}
bool gx::input::fire2() {
  return this->fired2;
}
bool gx::input::pickUp() {
  return this->pickup;
}
bool gx::input::switchW() {
  return this->switchWeapon;
}

move_t gx::input::movePlayer() {
  move_t movement = NULL_DIR;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    this->updated = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      movement = FORWARD_LEFT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      movement = FORWARD_RIGHT;
    } else {
      movement = FORWARD;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    this->updated = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      movement = BACKWARD_LEFT;
    } else {
      movement = LEFT;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    this->updated = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      movement = BACKWARD_RIGHT;
    } else {
      movement = RIGHT;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    this->updated = true;
    movement = BACKWARD;
  }
  return movement;
}

gx::vector3f gx::input::turnPlayer() {
  sf::Vector2i curPosition = sf::Mouse::getPosition();
  if(curPosition != mouseBasePosition) {
    this->updated = true;
    sf::Vector2i newDiff = curPosition - mouseBasePosition;
    mouseDiff += newDiff;
    mouseDirection = rotateZ(-mouseDiff.x * mouseSensitivity) * 
                     rotateX(-mouseDiff.y * mouseSensitivity) * basePlayerDirection;

    sf::Mouse::setPosition(mouseBasePosition);
  }
  return mouseDirection;
}

bool gx::input::resizedWindow() {
  return this->resized;
}

unsigned int gx::input::windowWidth() {
  return this->width;
}

unsigned int gx::input::windowHeight() {
  return this->height;
}

void gx::input::setUpMouse() {
  //somehow change this to set it to the center of the screen
  mouseBasePosition = sf::Vector2i(450,400);
  sf::Mouse::setPosition(mouseBasePosition);
}

void gx::input::handle(sf::Window& window) {
  this->updated = false;
  this->stopped = false;
  this->jumped  = false;
  this->resized = false;
  this->pickup = false;
  this->switchWeapon = false;
  sf::Event event;
  while (window.pollEvent(event)) {
    this->handleEvent(event);
  }
}

void gx::input::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::Closed) {
    this->stopped = true; // end the program
  } else if (event.type == sf::Event::Resized) {
    this->resized = true;
    this->width  = event.size.width,
    this->height = event.size.height;
    std::cout << "reshape w " << this->width << " h " << this->height << std::endl;
  } else if (event.type == sf::Event::KeyPressed) {
    if(event.key.code == sf::Keyboard::Escape) {
      this->stopped = true; // end the program
    } else if(event.key.code == sf::Keyboard::Space) {
      this->updated = true;
      this->jumped = true;
    } else if(event.key.code == sf::Keyboard::F){
      this->updated = true;
      this->pickup = true;
    } else if(event.key.code == sf::Keyboard::Q){
      this->updated = true;
      this->switchWeapon = true;
    }
  } else if(event.type == sf::Event::MouseButtonPressed) {
    this->updated = true;
	  if(event.mouseButton.button == sf::Mouse::Left) {
      this->fired1 = true;
	  } else if(event.mouseButton.button == sf::Mouse::Right) {
      this->fired2 = true;
    }
  } else if(event.type == sf::Event::MouseButtonReleased) {
    this->updated = true;
	  if(event.mouseButton.button == sf::Mouse::Left) {
      this->fired1 = false;
	  } else if(event.mouseButton.button == sf::Mouse::Right) {
      this->fired2 = false;
    }
  }
}
