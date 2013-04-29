#include "light.h"

gx::light::light(vector4 co, GLfloat ca, GLfloat la, GLfloat qa)
         : data{ std::array<GLfloat,4>(),
           std::array<GLfloat,4>{{ co.x, co.y, co.z, co.w }}, ca, la, qa },
           unif("light1",sizeof(lightData)) {
    unif.writeStruct(0,data);
  }

void gx::light::updatePosition(vector4 pos) {
  data.position = {{ pos.x, pos.y, pos.z, pos.w }};
  unif.write(0, data.position);
}

gx::uniform& gx::light::storage() {
  return this->unif;
}
