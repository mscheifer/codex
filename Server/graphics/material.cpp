#include "material.h"

namespace {
  constexpr GLenum diffuseBindPoint = 0;
}

gx::material::material(Texture diffuseT, vector4f diffuseCol)
  : diffuseTex(std::move(diffuseT)), diffuseColor(std::move(diffuseCol)) {}

void gx::material::bind() const {
  //this is the correct way to do it. Take GL_TEXTURE0 and add the number
  //look it up
  this->diffuseTex.bind(GL_TEXTURE0 + diffuseBindPoint);
}

void gx::material::setupBindings(shaderProgram const& program) {
  GLint diffuseLoc = program.uniformLoc("diffuseTex");
  this->program.use();
  glUniform1i(diffuseLoc, diffuseBindPoint); 
  debugout << "glUniform1i(" << diffuseLoc << ", " << diffuseBindPoint << ");";
  debugout << endl;
}
