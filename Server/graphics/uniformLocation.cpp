#include "uniformLocation.h"
#include "shaderProgram.h"

gx::uniform::loc<GL_FLOAT_MAT4>::loc(const shaderProgram& shader,
                                              std::string name,GLsizei num)
  : location(glGetUniformLocation(shader.progNum(), name.c_str())), size(num) {
  debugout << this->location << " = glGetUniformLocation(" << shader.progNum();
  debugout  << ", \"" << name << "\");" << endl;
  if(location == -1) {
    std::cout << "Error: no variable in shader named " << name << std::endl;
  }
}

void gx::uniform::loc<GL_FLOAT_MAT4>::write(const GLfloat* data) const {
  this->write(data,this->size);
}

void gx::uniform::loc<GL_FLOAT_MAT4>::write(const GLfloat* data,GLsizei s)const{
  glUniformMatrix4fv(this->location,s,false,data);
  debugout << "glUniformMatrix4fv(" << this->location << "," << s << ",false,";
  debugout << data << ");" << endl;
}

gx::uniform::loc<GL_FLOAT_VEC4>::loc(const shaderProgram& shader,
                                              std::string name, GLsizei num)
  : location(glGetUniformLocation(shader.progNum(), name.c_str())), size(num) {}

void gx::uniform::loc<GL_FLOAT_VEC4>::write(const GLfloat* data) const {
  this->write(data,this->size);
}

void gx::uniform::loc<GL_FLOAT_VEC4>::write(const GLfloat* data,GLsizei s)const{
  glUniform4fv(location,s,data);
}

gx::uniform::loc<GL_FLOAT>::loc(const shaderProgram& shader, std::string name,
                                                                 GLsizei num)
  : location(glGetUniformLocation(shader.progNum(), name.c_str())), size(num) {}

void gx::uniform::loc<GL_FLOAT>::write(const GLfloat* data) const {
  this->write(data,this->size);
}

void gx::uniform::loc<GL_FLOAT>::write(const GLfloat* data,GLsizei s) const {
  glUniform1fv(location,s,data);
}

void gx::uniform::loc<GL_FLOAT>::write(const GLfloat data) const {
  glUniform1f(location,data);
}

gx::uniform::loc<GL_UNSIGNED_INT>::loc(const shaderProgram& shader,
                                       std::string name, GLsizei num)
  : location(glGetUniformLocation(shader.progNum(), name.c_str())), size(num) {}

void gx::uniform::loc<GL_UNSIGNED_INT>::write(const GLuint* data) const {
  this->write(data,this->size);
}

void gx::uniform::loc<GL_UNSIGNED_INT>::write(const GLuint* data,GLsizei s) const {
  glUniform1uiv(location,s,data);
}

void gx::uniform::loc<GL_UNSIGNED_INT>::write(const GLuint data) const {
  glUniform1ui(location,data);
}
