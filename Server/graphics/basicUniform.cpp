#include "basicUniform.h"
#include <iostream>
#include <cstring>
#include "shaderProgram.h"

template<GLenum Type>
gx::uniform::_locations<Type>::_locations(std::string name,unsigned int s)
  : mSize(s), varName(std::move(name)) {}

template<GLenum Type>
void gx::uniform::_locations<Type>::updateLoc(GLuint shader,
                                      const GLfloat* data) const {
  assert(this->locations.find(shader) != this->locations.end());
  this->locations.find(shader)->second.write(data);
}

template<GLenum Type>
void gx::uniform::_locations<Type>::addShader(const shaderProgram& shader) {
  this->locations.insert(std::make_pair(shader.progNum(),
    loc<Type>(shader,this->varName,this->size())));
}

template<GLenum Type>
unsigned int gx::uniform::_locations<Type>::size() const {
  return this->mSize;
}

gx::uniform::mat4f::mat4f(std::string name)
  : _locations<GL_FLOAT_MAT4>(std::move(name),1) {}

void gx::uniform::mat4f::write(const GLfloat* src) {
  std::copy(src,src + this->storage.size(),this->storage.begin());
}

void gx::uniform::mat4f::write(const GLfloat) {
  std::cout << "Error: only one value passed" << std::endl;
  assert(false);
}

void gx::uniform::mat4f::write(const GLuint) {
  std::cout << "Error: GLuint passed" << std::endl;
  assert(false);
}

void gx::uniform::mat4f::update(GLuint shader) const {
  this->updateLoc(shader,this->storage.data());
}

std::string gx::uniform::mat4f::declaration() const {
  return "mat4 " + this->varName + ";";
}

gx::uniform::vec4f::vec4f(std::string name)
  : _locations<GL_FLOAT_VEC4>(std::move(name),1) {}

void gx::uniform::vec4f::write(const GLfloat* src) {
  std::copy(src,src + this->storage.size(),this->storage.begin());
}

void gx::uniform::vec4f::write(const GLfloat) {
  std::cout << "Error: only one value passed" << std::endl;
  assert(false);
}

void gx::uniform::vec4f::write(const GLuint) {
  std::cout << "Error: GLuint passed" << std::endl;
  assert(false);
}

void gx::uniform::vec4f::update(GLuint shader) const {
  this->updateLoc(shader,storage.data());
}

std::string gx::uniform::vec4f::declaration() const {
  return "vec4 " + this->varName + ";";
}

gx::uniform::vec4fArray::vec4fArray(std::string name, unsigned int size)
  : _locations<GL_FLOAT_VEC4>(std::move(name),size), storage(4*size) {}

void gx::uniform::vec4fArray::write(const GLfloat* src) {
  std::copy(src,src + this->storage.size(),this->storage.begin());
}

void gx::uniform::vec4fArray::write(const GLfloat) {
  std::cout << "Error: only one value passed" << std::endl;
  assert(false);
}

void gx::uniform::vec4fArray::write(const GLuint) {
  std::cout << "Error: GLuint passed" << std::endl;
  assert(false);
}

void gx::uniform::vec4fArray::update(GLuint shader) const {
  this->updateLoc(shader,this->storage.data());
}

std::string gx::uniform::vec4fArray::declaration() const {
  std::stringstream ss;
  ss << "vec4 " << this->varName << "[" << this->size() << "];";
  return ss.str();
}

gx::uniform::vec1f::vec1f(std::string name)
  : _locations<GL_FLOAT>(std::move(name),1) {}

void gx::uniform::vec1f::updateLoc(GLuint shader, const GLfloat data) const {
  this->locations.find(shader)->second.write(data);
}

void gx::uniform::vec1f::write(const GLfloat* src) {
  this->storage = *src;
}

void gx::uniform::vec1f::write(const GLfloat f) {
  this->storage = f;
}

void gx::uniform::vec1f::write(const GLuint) {
  std::cout << "Error: GLuint passed" << std::endl;
  assert(false);
}

void gx::uniform::vec1f::update(GLuint shader) const {
  this->updateLoc(shader,this->storage);
}

std::string gx::uniform::vec1f::declaration() const {
  return "float " + this->varName + ";";
}

gx::uniform::vec1u::vec1u(std::string name)
  : _locations<GL_UNSIGNED_INT>(std::move(name),1) {}

void gx::uniform::vec1u::updateLoc(GLuint shader, const GLuint data) const {
  this->locations.find(shader)->second.write(data);
}

void gx::uniform::vec1u::write(const GLfloat* src) {
  std::cout << "Error: GLfloat passed" << std::endl;
  assert(false);
}

void gx::uniform::vec1u::write(const GLfloat f) {
  std::cout << "Error: GLfloat passed" << std::endl;
  assert(false);
}

void gx::uniform::vec1u::write(const GLuint u) {
  this->storage = u;
}

void gx::uniform::vec1u::update(GLuint shader) const {
  this->updateLoc(shader,this->storage);
}

std::string gx::uniform::vec1u::declaration() const {
  return "uint " + this->varName + ";";
}

std::unique_ptr<gx::uniform::basic>
gx::uniform::make_uniform(std::string name, GLenum type, unsigned int size) {
  if(size == 1) {
    switch(type) {
      case GL_FLOAT_MAT4:
        return std::unique_ptr<basic>(new mat4f(std::move(name)));
      case GL_FLOAT_VEC4:
        return std::unique_ptr<basic>(new vec4f(std::move(name)));
      case GL_FLOAT:
        return std::unique_ptr<basic>(new vec1f(std::move(name)));
      case GL_UNSIGNED_INT:
        return std::unique_ptr<basic>(new vec1u(std::move(name)));
      default:
        std::cout << "Error: no make_uniform for: " << type << " " << size;
        std::cout << std::endl;
    }
  } else {
    assert(size > 1);
    switch(type) {
      case GL_FLOAT_VEC4:
        return std::unique_ptr<basic>(new vec4fArray(std::move(name),size));
      default:
        std::cout << "Error: no make_uniform for: " << type << " " << size;
        std::cout << std::endl;
    }
  }
  return std::unique_ptr<basic>(nullptr);
}

template class gx::uniform::_locations<GL_FLOAT_MAT4>;
template class gx::uniform::_locations<GL_FLOAT_VEC4>;
template class gx::uniform::_locations<GL_FLOAT>;
