#include "material.h"

gx::material::material(Texture diffuseT, vector4f diffuseCol)
  : diffuseTex(std::move(diffuseT)), diffuseColor(std::move(diffuseCol)) {}

void gx::material::bind() const {
  this->diffuseTex.bind(GL_TEXTURE0); //TODO: make the bind points constant
}
