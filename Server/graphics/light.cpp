#include "light.h"

namespace { //to not export
 const std::string uniformName = "light1";
 const std::array<std::pair<std::string,GLenum>,5> uniformVars = {{ 
   std::make_pair("light1position",       GL_FLOAT_VEC4), 
   std::make_pair("light1color",          GL_FLOAT_VEC4),
   std::make_pair("constantAttenuation",  GL_FLOAT),
   std::make_pair("linearAttenuation",    GL_FLOAT),
   std::make_pair("quadraticAttenuation", GL_FLOAT)
 }};

 std::map<std::string,GLenum> uniformVarsMap(
   uniformVars.begin(), uniformVars.end());
} //end unnamed namespace

gx::light::light(vector4f co, GLfloat ca, GLfloat la, GLfloat qa)
  : data(), unif(uniformName,uniformVarsMap) {
    //have to initialize here instead of initialization list because of visual
    //studio
    this->data.constantAttenuation = ca;
    this->data.linearAttenuation = la;
    this->data.quadraticAttenuation = qa;
    this->data.color[0] = co.x;
    this->data.color[1] = co.y;
    this->data.color[2] = co.z;
    this->data.color[3] = co.w;
    //clean these up a bit:
    this->unif.write(uniformVars[1].first, data.color);
    this->unif.write(uniformVars[2].first, data.constantAttenuation);
    this->unif.write(uniformVars[3].first, data.linearAttenuation);
    this->unif.write(uniformVars[4].first, data.quadraticAttenuation);
    //this->unif.writeBufferStruct(0,data); TODO: uncomment and renenable
  }

void gx::light::updatePosition(vector4f pos) {
  std::array<GLfloat,4> newpos = {{ pos.x, pos.y, pos.z, pos.w }};
  this->data.position = newpos;
  this->unif.write(uniformVars[0].first, data.position);
}

gx::uniform::block& gx::light::storage() {
  return this->unif;
}
