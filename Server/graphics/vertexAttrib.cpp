#include "vertexAttrib.h"

gx::vertexAttrib::vertexAttrib(vertexAttrib&& other) noexcept
  : varName(std::move(other.varName)), vectorSize(std::move(other.vectorSize)),
    glType(std::move(other.glType)), byteOffset(std::move(other.byteOffset)),
    buff(std::move(other.buff)) {}


void gx::vertexAttrib::bindBuffer() const {
  this->buff.bind(GL_ARRAY_BUFFER);
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
