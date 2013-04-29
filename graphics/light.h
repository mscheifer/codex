#include <array>
#include "vector4.h"
#include "uniform.h"

namespace gx {

class light {
    struct lightData {
      std::array<GLfloat,4> position;
      std::array<GLfloat,4> color;
      GLfloat               constantAttenuation;
      GLfloat               linearAttenuation;
      GLfloat               quadraticAttenuation;
    } data;
    uniform unif;
  public:
    light(vector4,GLfloat,GLfloat,GLfloat);
    void updatePosition(vector4);
    uniform& storage();
};

}
