#include "ParticleGenerator.h"


gx::ParticleGenerator::ParticleGenerator() 
{
  this->ParticleGenerator::ParticleGenerator(vector3f(0,0,5));
}

gx::ParticleGenerator::ParticleGenerator(vector3f pos) : 
  allParticles(ParticleCount, Particle(pos)), position(pos)
{
  
}
                                              
gx::ParticleGenerator::~ParticleGenerator() {}

void gx::ParticleGenerator::move(vector3f posDiff) {
  this->position += posDiff;
  this->run();
}

void gx::ParticleGenerator::run() {
  //for (auto p = allParticles.begin(); p != allParticles.end(); ++p) {
  //  p->run();
  //}
}

void gx::ParticleGenerator::stop() {

}