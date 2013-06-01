#include <GL/glew.h>
#include <array>
#include "vector4.h"
#include "uniformBlock.h"

namespace gx {

struct light {
    static const unsigned int maxLights = 10;
  private:
    struct lightData {
      std::array<GLfloat,4 * maxLights> position;
      std::array<GLfloat,4> color;
      GLuint                numLights;
      GLfloat               constantAttenuation;
      GLfloat               linearAttenuation;
      GLfloat               quadraticAttenuation;
    } data;
    uniform::block unif;
  public:
    void clear();
    light(vector4f,GLfloat,GLfloat,GLfloat);
    void addLight(vector4f);
    uniform::block& storage();
};

}
