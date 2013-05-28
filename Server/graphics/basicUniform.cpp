#include "basicUniform.h"
#include <iostream>
#include <cstring>
#include "shaderProgram.h"

template<GLenum Type>
gx::uniform::_locations<Type>::_locations(std::string name)
  : varName(std::move(name)) {}

template<GLenum Type>
void gx::uniform::_locations<Type>::updateLoc(GLuint shader,
                                      const GLfloat* data) const {
  this->locations.at(shader).write(data);
}


template<GLenum Type>
void gx::uniform::_locations<Type>::addShader(const shaderProgram& shader) {
  this->locations.insert(
    std::make_pair(shader.progNum(),loc<Type>(shader,this->varName,1)));
}

gx::uniform::mat4f::mat4f(std::string name)
  : _locations<GL_FLOAT_MAT4>(std::move(name)) {}

void gx::uniform::mat4f::write(const GLfloat* src) {
  std::memcpy(this->storage.data(),src,sizeof(this->storage));
}

void gx::uniform::mat4f::write(const GLfloat) {
  std::cout << "Error: only one value passed" << std::endl;
}

void gx::uniform::mat4f::update(GLuint shader) const {
  this->updateLoc(shader,this->storage.data());
}

std::string gx::uniform::mat4f::declaration() const {
  return "mat4 " + this->varName + ";";
}

gx::uniform::vec4f::vec4f(std::string name)
  : _locations<GL_FLOAT_VEC4>(std::move(name)) {}

void gx::uniform::vec4f::write(const GLfloat* src) {
  std::memcpy(this->storage.data(),src,sizeof(this->storage));
}

void gx::uniform::vec4f::write(const GLfloat) {
  std::cout << "Error: only one value passed" << std::endl;
}

void gx::uniform::vec4f::update(GLuint shader) const {
  this->updateLoc(shader,storage.data());
}

std::string gx::uniform::vec4f::declaration() const {
  return "vec4 " + this->varName + ";";
}

gx::uniform::vec1f::vec1f(std::string name)
  : _locations<GL_FLOAT>(std::move(name)) {}

void gx::uniform::vec1f::updateLoc(GLuint shader, const GLfloat data) const {
  this->locations.at(shader).write(data);
}

void gx::uniform::vec1f::write(const GLfloat* src) {
  this->storage = *src;
}

void gx::uniform::vec1f::write(const GLfloat f) {
  this->storage = f;
}

void gx::uniform::vec1f::update(GLuint shader) const {
  this->updateLoc(shader,this->storage);
}

std::string gx::uniform::vec1f::declaration() const {
  return "float " + this->varName + ";";
}

std::unique_ptr<gx::uniform::basic>
gx::uniform::make_uniform(std::string name, GLenum type) {
  switch(type) {
    case GL_FLOAT_MAT4:
      return std::unique_ptr<basic>(new mat4f(std::move(name)));
    case GL_FLOAT_VEC4:
      return std::unique_ptr<basic>(new vec4f(std::move(name)));
    case GL_FLOAT:
      return std::unique_ptr<basic>(new vec1f(std::move(name)));
    default:
      std::cout << "Error: no make_uniform for: " << type << std::endl;
  }
  return std::unique_ptr<basic>(nullptr);
}

template class gx::uniform::_locations<GL_FLOAT_MAT4>;
template class gx::uniform::_locations<GL_FLOAT_VEC4>;
template class gx::uniform::_locations<GL_FLOAT>;
