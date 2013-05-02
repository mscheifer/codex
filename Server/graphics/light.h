#include <GL/glew.h>
#include <array>
#include "vector4.h"
#include "globalUniform.h"

namespace gx {

class light {
    struct lightData {
      std::array<GLfloat,4> position;
      std::array<GLfloat,4> color;
      GLfloat               constantAttenuation;
      GLfloat               linearAttenuation;
      GLfloat               quadraticAttenuation;
    } data;
    globalUniform unif;
  public:
    light(vector4,GLfloat,GLfloat,GLfloat);
    void updatePosition(vector4);
    globalUniform& storage();
};

}
