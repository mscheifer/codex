#ifndef PARTICLE_H
#define PARTICLE_H
#include "vector3.h"

namespace gx {

  class Particle {

  private:
    float lifespan; // counts down to 0 until disappears
    vector3f position;
    vector3f velocity;
    vector3f acceleration;

    void initRandVelAccl();
    void update();
    void draw();
    
  public:
    Particle(vector3f pos);
    Particle();
    //Particle(const Particle&);// = delete;
    Particle& operator=(const Particle&);// = delete;
    Particle(Particle&&);// = delete;
    Particle& operator=(Particle&&);// = delete;
    ~Particle();
    
    void run();
    bool isDead();
    void reborn(vector3f pos);

  };

}

#endif