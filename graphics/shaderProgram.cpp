#include "shaderProgram.h"
#include <array>
#include <iostream>
#include <fstream>

namespace { //to not export
void printShaderInfoLog(GLuint obj,const std::string name) {
  GLint   infoLogLength = 0;
  GLsizei charsWritten  = 0;
  GLchar* infoLog;

  glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infoLogLength);

  if (infoLogLength > 1) {
    infoLog = new GLchar[infoLogLength];
    glGetShaderInfoLog(obj, infoLogLength, &charsWritten, infoLog);
    std::cout << name << ": errors " << infoLogLength << std::endl;
    std::cout << infoLog << std::endl;
    delete[] infoLog;
  }
}

void printProgramInfoLog(GLuint obj) {
  GLint   infoLogLength = 0;
  GLsizei charsWritten  = 0;
  GLchar* infoLog;

  glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infoLogLength);

  if (infoLogLength > 1) {
    infoLog = new GLchar[infoLogLength];
    glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog);
    std::cout << "Program: errors" << std::endl;
    std::cout << infoLog << std::endl;
    delete[] infoLog;
  }
}

std::string readFile(const std::string fileName) {
  std::ifstream vsFile(fileName);
  std::string fullSource = "";

  if(vsFile.is_open()) {
    std::string line;
    while(vsFile.good()) {
      getline(vsFile,line);
      fullSource += line + "\n";
    }
    vsFile.close();
  }
  return fullSource;
}

} //end unnamed namespace

gx::shaderProgram::shaderProgram(std::string vsSource,std::string fsSource,
        const std::vector<std::pair<const std::string,GLuint>> uniforms,
        const std::vector<std::pair<const std::string,GLuint>> attribs)
                 : prog(glCreateProgram()) {
  debugout << prog << " = glCreateProgram()" << endl;
  const std::string shader_output_name("outputF");

  GLuint vertShader,fragShader;

  vertShader = glCreateShader(GL_VERTEX_SHADER);
  debugout << vertShader << " = glCreateShader(GL_VERTEX_SHADER);" << endl;
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  debugout << fragShader << " = glCreateShader(GL_FRAGMENT_SHADER);" << endl;

  //include version string and stuff
  std::array<const GLchar*,2> fullSourceV =
    {{ shaderHeader.c_str(), vsSource.c_str() }};
  std::array<const GLchar*,2> fullSourceF =
    {{ shaderHeader.c_str(), fsSource.c_str() }};

  glShaderSource(vertShader, fullSourceV.size(), fullSourceV.data(), nullptr);
  glShaderSource(fragShader, fullSourceF.size(), fullSourceF.data(), nullptr);

  glCompileShader(vertShader);
  glCompileShader(fragShader);

  printShaderInfoLog(vertShader,"vert");
  printShaderInfoLog(fragShader,"frag");

  glAttachShader(this->prog,vertShader);
  glAttachShader(this->prog,fragShader);

  //we have to do this here because we're stuck on GLSL 1.3
  glBindFragDataLocation(prog, 0, shader_output_name.c_str());

  //cant use range based for here because of visual c++
  for(auto attribp = attribs.begin() ; attribp != attribs.end() ; ++attribp) {
	const auto& attrib = *attribp;
    debugout << "glBindAttribLocation(" << this->prog << ", " << attrib.second;
    debugout << ", " << attrib.first.c_str() << ");" << endl;
    glBindAttribLocation(this->prog,attrib.second,attrib.first.c_str());
  }

  debugout << "glLinkProgram(" << this->prog << ");" << endl;
  glLinkProgram(this->prog);
  printProgramInfoLog(this->prog);

  if(glGetFragDataLocation(prog,shader_output_name.c_str()) == -1) {
    std::cout << "Error, bad output for frag shader" << std::endl;
  }

  glDetachShader(this->prog,vertShader);
  glDetachShader(this->prog,fragShader);

  //we already linked so we don't need these anymore
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

   //cant use range based for here because of visual c++
  for(auto uniformp = uniforms.begin() ; uniformp != uniforms.end() ; ++uniformp) {
    const auto& uniform = *uniformp;
    //uniform.first is the block name, second is the block binding id
    GLuint localIndex= glGetUniformBlockIndex(this->prog,uniform.first.c_str());
    if(localIndex == GL_INVALID_INDEX) {
      std::cout << uniform.first << ": doesn't exist in shader" << endl;
    }

    debugout << localIndex << " = glGetUniformBlockIndex(" << this->prog;
    debugout << ", \"" << uniform.first.c_str() << "\");" << endl;
    debugout << "glUniformBlockBinding(" << this->prog << ", " << localIndex;
    debugout << ", " << uniform.second << ");" << endl;
    glUniformBlockBinding(this->prog,localIndex,uniform.second);

    int sz;
    glGetActiveUniformBlockiv(this->prog, localIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &sz);
    debugout << "uniform size: " << sz << endl;
  }
}

gx::shaderProgram::shaderProgram(gx::shaderProgram&& other) {
	this->prog = other.prog;
	other.prog = 0; //glDeleteProgram wont do anything
}

gx::shaderProgram::~shaderProgram() {
  glDeleteProgram(this->prog);
  debugout << "glDeleteProgram(" << this->prog << ");" << endl;
}

void gx::shaderProgram::use() {
  glUseProgram(this->prog);
  debugout << "glUseProgram(" << this->prog << ");" << endl;
}

gx::shaderProgram gx::shaderFromFiles(const std::string vsFile,
                                      const std::string fsFile,
           const std::vector<std::pair<const std::string,GLuint>> uniforms,
           const std::vector<std::pair<const std::string,GLuint>> attribs) {
  std::string vsSource, fsSource;

  vsSource = readFile(vsFile);
  fsSource = readFile(fsFile);

  return shaderProgram(vsSource,fsSource,uniforms,attribs);
}
