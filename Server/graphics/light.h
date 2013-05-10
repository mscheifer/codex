#include <GL/glew.h>
#include <array>
#include "vector4.h"
#include "uniformBlock.h"

namespace gx {

class light {
    struct lightData {
      std::array<GLfloat,4> position;
      std::array<GLfloat,4> color;
      GLfloat               constantAttenuation;
      GLfloat               linearAttenuation;
      GLfloat               quadraticAttenuation;
    } data;
    uniform::block unif;
  public:
    light(vector4f,GLfloat,GLfloat,GLfloat);
    void updatePosition(vector4f);
    uniform::block& storage();
};

}
