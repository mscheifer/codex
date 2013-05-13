#include "uniformLocation.h"
#include <string>
#include "shaderProgram.h"

gx::uniform::loc<GL_FLOAT_MAT4>::loc(const shaderProgram& shader,std::string name)
  : location(glGetUniformLocation(shader.progNum(), name.c_str())) {}

void gx::uniform::loc<GL_FLOAT_MAT4>::write(const GLfloat* data) const {
  glUniformMatrix4fv(location,1,false,data);
  debugout << "glUniformMatrix4fv(" << location << ",1,false," << data << ");";
  debugout << endl;
}

gx::uniform::loc<GL_FLOAT_VEC4>::loc(const shaderProgram& shader,std::string name)
  : location(glGetUniformLocation(shader.progNum(), name.c_str())) {}

void gx::uniform::loc<GL_FLOAT_VEC4>::write(const GLfloat* data) const {
  glUniform4fv(location,1,data);
}

gx::uniform::loc<GL_FLOAT>::loc(const shaderProgram& shader,std::string name)
  : location(glGetUniformLocation(shader.progNum(), name.c_str())) {}

void gx::uniform::loc<GL_FLOAT>::write(const GLfloat* data) const {
  glUniform1fv(location,1,data);
}

void gx::uniform::loc<GL_FLOAT>::write(const GLfloat data) const {
  glUniform1f(location,data);
}
