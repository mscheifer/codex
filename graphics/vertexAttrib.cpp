#include "vertexAttrib.h"

gx::vertexAttrib::~vertexAttrib() {
  glDeleteBuffers(1, &buffer);
  debugout << "glDeleteBuffers(1, &buffer@" << &buffer << ");" << endl;
}

void gx::vertexAttrib::bindBuffer() const {
  glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
  debugout << "glBindBuffer(GL_ARRAY_BUFFER, " << this->buffer << ");" << endl;
}

std::string gx::vertexAttrib::name() const {
  return this->varName;
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
