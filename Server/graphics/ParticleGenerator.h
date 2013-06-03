#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include "Particle.h"

namespace {
  const unsigned int ParticleCount = 500;
}

namespace gx {
  class ParticleGenerator
  {
  private:
    std::vector<Particle> allParticles;
    vector3f position;


  public:
    ParticleGenerator(vector3f pos);
    ParticleGenerator();
    ~ParticleGenerator();
    //ParticleGenerator(const ParticleGenerator&);// = delete;
    //ParticleGenerator& operator=(const ParticleGenerator&);// = delete;
    //ParticleGenerator(ParticleGenerator&&);// = delete;
    //ParticleGenerator& operator=(ParticleGenerator&&);// = delete;

    void move(vector3f posDiff);
    void run();
    void stop();
  };
}

#endif