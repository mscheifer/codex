#include "Particle.h"
#include <math.h>
#include <stdlib.h>
#include "vector2.h"

gx::Particle::Particle(vector3f pos) : lifespan(255), position(pos), 
                                       velocity(), acceleration()
{
  initRandVelAccl();
}

gx::Particle::Particle() {
  this->Particle::Particle(vector3f(0,0,5));
}

gx::Particle::~Particle() {}

gx::Particle& gx::Particle::operator=(Particle&& other) {
  this->lifespan = std::move(other.lifespan);
  this->position = std::move(other.position);
  this->velocity = std::move(other.velocity);
  this->acceleration = std::move(other.acceleration);
}

void gx::Particle::initRandVelAccl() {
  this->acceleration.z = (float)(rand() % 10) / 100;

  vector2f rangeX(-1, 1);
  this->velocity.x = (*rangeX.begin()) + (float)rand()/
                     ((float)RAND_MAX/((*rangeX.end())-(*rangeX.begin())));

  vector2f rangeY(-1, 1);
  this->velocity.y = (*rangeY.begin()) + (float)rand()/
                     ((float)RAND_MAX/((*rangeY.end())-(*rangeY.begin())));
  
  vector2f rangeZ(0, 1);
  this->velocity.y = (*rangeZ.begin()) + (float)rand()/
                     ((float)RAND_MAX/((*rangeZ.end())-(*rangeZ.begin())));
}

void gx::Particle::update() {
  velocity += acceleration;
  position += velocity;

  lifespan -= 2.0;
}

void gx::Particle::draw() {
  // use the lifespan float also as an alpha value
}

void gx::Particle::run() {
  this->update();
  this->draw();
}

bool gx::Particle::isDead() {
  return lifespan < 0.0f;
}

