#include "uniform.h"
#include "oglUtil.h"

//stores the next available uniform bind point
//should be incremented every time it is used
GLuint gx::uniform::nextUniformBindPoint = 0;

GLuint gx::uniform::freshBindPoint() {
  if(nextUniformBindPoint >= GL_MAX_UNIFORM_BUFFER_BINDINGS) {
    std::cout << "Error, too many uniform objects" << std::endl;
    exit(1);
  }
  return nextUniformBindPoint++;
}

gx::uniform::uniform(std::string n, GLsizeiptr buffSize)
  : varName(n), bindingIndex(freshBindPoint()), bufferName() {
  glGenBuffers(1, &(this->bufferName));
  debugout << "glGenBuffers(1, &(this->bufferName));" << endl;
  glBindBuffer(GL_UNIFORM_BUFFER, this->bufferName);
  debugout << "glBindBuffer(GL_UNIFORM_BUFFER, " << this->bufferName << ");";
  debugout << endl;
  glBufferData(GL_UNIFORM_BUFFER, buffSize, nullptr, GL_DYNAMIC_DRAW);
  debugout << "glBufferData(GL_UNIFORM_BUFFER, " << buffSize << ", nullptr, ";
  debugout << "GL_DYNAMIC_DRAW);" << endl;
  glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingIndex, this->bufferName);
  debugout << "glBindBufferBase(GL_UNIFORM_BUFFER, " << this->bindingIndex;
  debugout << ", " << this->bufferName << ");" << endl;
}

gx::uniform::~uniform() {
  debugout << "glDeleteBuffers(1, &(this->bufferName));" << endl;
  glDeleteBuffers(1, &(this->bufferName));
}

void gx::uniform::write(GLintptr offset, GLsizeiptr size,
                        const GLvoid* data) const {
  debugout << "glBindBuffer(GL_UNIFORM_BUFFER, " << this->bufferName << ");";
  debugout << endl;
  glBindBuffer(GL_UNIFORM_BUFFER, this->bufferName);
  debugout << "glBufferSubData(GL_UNIFORM_BUFFER, " << offset << ", ";
  debugout << size << ", oglM.data());" << endl;
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

std::string gx::uniform::name() const {
  return this->varName;
}

GLuint gx::uniform::bindPoint() const {
  return this->bindingIndex;
}
