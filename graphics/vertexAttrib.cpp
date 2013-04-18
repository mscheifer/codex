#include "vertexAttrib.h"

//stores the next available uniform bind point
//should be incremented every time it is used
GLuint gx::vertexAttrib::nextAttribLocation = 0;

GLuint gx::vertexAttrib::freshAttribLocation() {
  if(nextAttribLocation >= GL_MAX_VERTEX_ATTRIBS) {
    std::cout << "Error, too many vertex attributes" << std::endl;
    exit(1);
  }
  return nextAttribLocation++;
}

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
