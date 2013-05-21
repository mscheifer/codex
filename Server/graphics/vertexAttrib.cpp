#include "vertexAttrib.h"

gx::vertexAttrib::vertexAttrib(vertexAttrib&& other) noexcept
  : varName(std::move(other.varName)), vectorSize(std::move(other.vectorSize)),
    glType(std::move(other.glType)), byteOffset(std::move(other.byteOffset)),
    buffer(std::move(other.buffer)) {
  other.buffer = 0; //destructor won't do anything  
}

gx::vertexAttrib::~vertexAttrib() noexcept {
  glDeleteBuffers(1, &(this->buffer));
  debugout << "vert glDeleteBuffers(1, &(this->buffer): " << this->buffer << ");" << endl;
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
