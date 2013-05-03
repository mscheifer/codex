#include "light.h"

gx::light::light(vector4 co, GLfloat ca, GLfloat la, GLfloat qa)
         : data(), unif("light1",std::map<std::string,GLenum>()) { //TODO: fill map
    //have to initialize here instead of initialization list because of visual studio
    this->data.constantAttenuation = ca;
    this->data.linearAttenuation = la;
    this->data.quadraticAttenuation = qa;
    this->data.color[0] = co.x;
    this->data.color[1] = co.y;
    this->data.color[2] = co.z;
    this->data.color[3] = co.w;
    //this->unif.writeStruct(0,data); TODO: uncomment and renenable
  }

void gx::light::updatePosition(vector4 pos) {
  std::array<GLfloat,4> newpos = {{ pos.x, pos.y, pos.z, pos.w }};
  data.position = newpos;
  unif.write(0, data.position);
}

gx::uniform::block& gx::light::storage() {
  return this->unif;
}
