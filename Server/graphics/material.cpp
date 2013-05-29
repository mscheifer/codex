#include "material.h"

gx::material::material(Texture diffuseT, vector4f diffuseCol)
  : diffuseTex(std::move(diffuseT)), diffuseColor(std::move(diffuseCol)) {}

void gx::material::bind() const {
  //this is the correct way to do it. Take GL_TEXTURE0 and add the number
  //look it up
  this->diffuseTex.bind(GL_TEXTURE0 + 0); //TODO: make the bind points constant
}
