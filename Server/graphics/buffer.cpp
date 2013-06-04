#include "buffer.h"

gx::buffer::buffer(buffer&& other)
  : name(std::move(other.name)) {
  other.name = 0; //destructor won't do anything 
}

gx::buffer::~buffer() noexcept {
  glDeleteBuffers(1, &(this->name));
  debugout << "glDeleteBuffers(1, &(this->name): " << this->name << ");";
  debugout << endl;
}

void gx::buffer::bind() const {
  glBindBuffer(this->target, this->name);
  debugout << "glBindBuffer(" << this->target << ", " << this->name << ");" << endl;
}