#include "vertexAttrib.h"

void gx::vertexAttrib::bindBuffer() const {
  debugout << "glBindBuffer(GL_ARRAY_BUFFER, " << this->buffer << ");" << endl;
  glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
}

GLuint gx::vertexAttrib::loc() const {
  return this->shaderLocation;
}

GLint gx::vertexAttrib::vecSize() const {
  return this->vectorSize;
}

GLenum gx::vertexAttrib::type() const {
  return this->glType;
}

GLsizei gx::vertexAttrib::stride() const {
  return this->byteOffset;
}
