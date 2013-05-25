#include "shaderProgram.h"
#include <array>
#include <iostream>
#include "uniformBlock.h"

namespace { //to not export
void printShaderInfoLog(GLuint obj,const std::string name) {
  GLint   infoLogLength = 0;
  GLsizei charsWritten  = 0;

  glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infoLogLength);

  if (infoLogLength > 1) {
    std::vector<GLchar> infoLog;
    infoLog.resize(infoLogLength);
    glGetShaderInfoLog(obj, infoLogLength, &charsWritten, infoLog.data());
    std::cout << name << ": compile output " << infoLogLength << std::endl;
    std::cout << std::string(infoLog.begin(),infoLog.end()) << std::endl;
  }
}

void printProgramInfoLog(GLuint obj) {
  GLint   infoLogLength = 0;
  GLsizei charsWritten  = 0;

  glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infoLogLength);

  if (infoLogLength > 1) {
    std::vector<GLchar> infoLog;
    infoLog.resize(infoLogLength);
    glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog.data());
    std::cout << "Program: errors" << std::endl;
    std::cout << std::string(infoLog.begin(),infoLog.end()) << std::endl;
  }
}

} //end unnamed namespace

gx::shaderProgram::shaderProgram(   const std::string vsSource,
        const std::string fsSource, const std::vector<uniform::block*> uniforms)
                 : prog(glCreateProgram()), attribSigs() {
  debugout << prog << " = glCreateProgram()" << endl;
  const std::string shader_output_name("outputF");

  GLuint vertShader,fragShader;

  vertShader = glCreateShader(GL_VERTEX_SHADER);
  debugout << vertShader << " = glCreateShader(GL_VERTEX_SHADER);" << endl;
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  debugout << fragShader << " = glCreateShader(GL_FRAGMENT_SHADER);" << endl;

  std::string uniformDecl = "";
  for(auto uniformp = uniforms.begin(); uniformp != uniforms.end(); ++uniformp){
    uniformDecl += (*uniformp)->declaration();
  }

  //include version string and stuff
  std::array<const GLchar*,3> fullSourceV =
    {{ shaderHeader.c_str(), uniformDecl.c_str(), vsSource.c_str() }};
  std::array<const GLchar*,3> fullSourceF =
    {{ shaderHeader.c_str(), uniformDecl.c_str(), fsSource.c_str() }};

  glShaderSource(vertShader, GLsizei(fullSourceV.size()), fullSourceV.data(),
                                                                    nullptr);
  glShaderSource(fragShader, GLsizei(fullSourceF.size()), fullSourceF.data(),
                                                                    nullptr);

  glCompileShader(vertShader);
  glCompileShader(fragShader);

  printShaderInfoLog(vertShader,"vert");
  printShaderInfoLog(fragShader,"frag");

  glAttachShader(this->prog,vertShader);
  glAttachShader(this->prog,fragShader);

  //we have to do this here because we're stuck on GLSL 1.3
  glBindFragDataLocation(prog, 0, shader_output_name.c_str());

  glLinkProgram(this->prog);
  debugout << "glLinkProgram(" << this->prog << ");" << endl;
  printProgramInfoLog(this->prog);

  if(glGetFragDataLocation(this->prog,shader_output_name.c_str()) == -1) {
    std::cout << "Error, bad output for frag shader" << std::endl;
  }
  debugout << "glGetFragDataLocation(" << this->prog << ", ";
  debugout << shader_output_name << ");" << endl;

  glDetachShader(this->prog,vertShader);
  debugout << "glDetachShader("<< this->prog<< ","<< vertShader << ");" << endl;
  glDetachShader(this->prog,fragShader);
  debugout << "glDetachShader("<< this->prog<< ","<< fragShader << ");" << endl;

  //we already linked so we don't need these anymore
  glDeleteShader(vertShader);
  debugout << "glDeleteShader(" << vertShader << ");" << endl;
  glDeleteShader(fragShader);
  debugout << "glDeleteShader(" << fragShader << ");" << endl;

  GLint maxAttribNameLength, numAttribs;

  glGetProgramiv(this->prog, GL_ACTIVE_ATTRIBUTES, &numAttribs);
  debugout << "glGetProgramiv(" << this->prog << ", GL_ACTIVE_ATTRIBUTES, ";
  debugout << "&numAttribs@" << &numAttribs << ");" << endl;
  debugout << "numAttribs = " << numAttribs << endl;
  glGetProgramiv(this->prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                                                   &maxAttribNameLength);
  debugout << "glGetProgramiv(" << this->prog << ", ";
  debugout << "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength@";
  debugout << &maxAttribNameLength << ");" << endl;
  debugout << "maxAttribNameLength = " << maxAttribNameLength << endl;

  std::vector<GLchar> attribName;
  attribName.resize(maxAttribNameLength);

  for(GLuint i = 0; i < GLuint(numAttribs); ++i) {
    GLsizei writtenChars;
    GLint attribSize;
    GLenum attribType;
    glGetActiveAttrib(this->prog, i, maxAttribNameLength, &writtenChars, &attribSize,
                      &attribType, attribName.data());
    debugout << "glGetActiveAttrib(" << this->prog << ", " << i << ", ";
    debugout << maxAttribNameLength << ", nullptr, &attribSize@" << &attribSize;
    debugout << ", &attribType@" << &attribType << ", ";
    debugout << std::string(attribName.begin(), attribName.begin() + writtenChars) << ");" << endl;
    GLint attribLoc = glGetAttribLocation(this->prog, attribName.data());
    debugout << attribLoc << " = glGetAttribLocation(" << this->prog << ", ";
    debugout << std::string(attribName.begin(), attribName.begin() + writtenChars) << ");" << endl;
    vertexAttribSignature sig(attribType,attribLoc);
    this->attribSigs.insert(std::make_pair(std::string(attribName.begin(),attribName.begin() + writtenChars),sig));
  }

  for(auto uniformp = uniforms.begin(); uniformp != uniforms.end(); ++uniformp){
    auto& unif = **uniformp;
    unif.addShaderBindings(this);
  }
}

gx::shaderProgram::shaderProgram(gx::shaderProgram&& other): prog(other.prog) {
  other.prog = 0; //glDeleteProgram wont do anything
}

gx::shaderProgram& gx::shaderProgram::operator=(gx::shaderProgram&& other) {
  this->prog = other.prog;
  other.prog = 0; //glDeleteProgram wont do anything
  return *this;
}

gx::shaderProgram::~shaderProgram() {
  glDeleteProgram(this->prog);
  debugout << "glDeleteProgram(" << this->prog << ");" << endl;
}

void gx::shaderProgram::use() const {
  glUseProgram(this->prog);
  debugout << "glUseProgram(" << this->prog << ");" << endl;
}

std::map<std::string,gx::vertexAttribSignature> gx::shaderProgram::vars() const{
  return this->attribSigs;
}

GLint gx::shaderProgram::uniformLoc(const std::string name) const {
  return glGetUniformLocation(this->prog, name.c_str());
}

GLuint gx::shaderProgram::progNum() const {
  return this->prog;
}
