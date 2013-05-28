#include "material.h"

gx::material::material(Texture diffuseT, vector4f diffuseCol)
  : diffuseTex(std::move(diffuseT)), diffuseColor(std::move(diffuseCol)) {}

void gx::material::bind() {
  this->diffuseTex.bind(GL_TEXTURE0);
}
