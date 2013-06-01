#include "light.h"

namespace { //to not export
 const std::string uniformName = "light1";
 //we can't use tuples so pair with a pair
 const std::array<gx::uniform::sigs_t,6> uniformVars = {{ 
   std::make_pair("lightPosition",       std::make_pair(GL_FLOAT_VEC4,gx::light::maxLights)), 
   std::make_pair("lightColor",          std::make_pair(GL_FLOAT_VEC4,1)),
   std::make_pair("numLights",           std::make_pair(GL_UNSIGNED_INT,1)),
   std::make_pair("constantAttenuation", std::make_pair(GL_FLOAT,1)),
   std::make_pair("linearAttenuation",   std::make_pair(GL_FLOAT,1)),
   std::make_pair("quadraticAttenuation",std::make_pair(GL_FLOAT,1))
 }};

 std::map<gx::uniform::name_t,gx::uniform::type_t> uniformVarsMap(
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
  this->clear();
  this->unif.write(uniformVars[1].first, this->data.color);
  this->unif.write(uniformVars[3].first, this->data.constantAttenuation);
  this->unif.write(uniformVars[4].first, this->data.linearAttenuation);
  this->unif.write(uniformVars[5].first, this->data.quadraticAttenuation);
  //this->unif.writeBufferStruct(0,data); TODO: uncomment and renenable
}

void gx::light::clear() {
  this->data.numLights = 0;
  this->unif.write(uniformVars[2].first, this->data.numLights);
}

void gx::light::addLight(vector4f pos) {
  if(this->data.numLights < maxLights) {
    auto newpos = pos.oglVec4();
    auto startItr = this->data.position.begin() + (this->data.numLights * 4);
    std::copy(newpos.begin(),newpos.end(),startItr);
    this->data.numLights++;
    this->unif.write(uniformVars[2].first, this->data.numLights);
    this->unif.write(uniformVars[0].first, data.position);
  } else {
    std::cout << "warning too many lights" << std::endl;
  }
}

gx::uniform::block& gx::light::storage() {
  return this->unif;
}
