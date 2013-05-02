#include "uniformBlock.h"
#include "oglUtil.h"

//stores the next available uniform bind point
//should be incremented every time it is used
GLuint gx::uniform::block::nextUniformBindPoint = 0;

GLuint gx::uniform::block::freshBindPoint() {
  if(nextUniformBindPoint >= GL_MAX_UNIFORM_BUFFER_BINDINGS) {
    std::cout << "Error, too many uniform buffers" << std::endl;
    exit(1);
  }
  return nextUniformBindPoint++;
}

gx::uniform::block::block(std::string n, std::map<std::string,GLenum> varNameType)
  : blockName(n), bindingIndex(freshBindPoint()), bufferName(-5), basicStorage(), offsets(), basicLocations() {
  if(gx::sharedUniformsOn) {
    GLsizeiptr buffSize = -3;//todo fix this

	//setup this->offsets

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
  } else {
    for(auto varp = varNameType.begin(); varp != varNameType.end(); varp++) {
      basicStorage.push_back(make_uniform(varp->first,varp->second));
    }
  }
}

gx::uniform::block::~block() {
  if(gx::sharedUniformsOn) {
    glDeleteBuffers(1, &(this->bufferName));
    debugout << "glDeleteBuffers(1, &(this->bufferName): " << this->bufferName;
    debugout << ");" << endl;
  }
}

void gx::uniform::block::write(GLintptr offset, GLsizeiptr size,
                        const GLvoid* data) const {
  debugout << "glBindBuffer(GL_UNIFORM_BUFFER, " << this->bufferName << ");";
  debugout << endl;
  glBindBuffer(GL_UNIFORM_BUFFER, this->bufferName);
  debugout << "glBufferSubData(GL_UNIFORM_BUFFER, " << offset << ", ";
  debugout << size << ", data@" << data << ");" << endl;
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void gx::uniform::block::addShaderBindings(shaderProgram* p) {
  GLuint prog = p->progNum();
  if(gx::sharedUniformsOn) {
    GLuint localIndex = glGetUniformBlockIndex(prog,this->name().c_str());
    debugout << localIndex << " = glGetUniformBlockIndex(" << prog;
    debugout << ", \"" << this->name().c_str() << "\");" << endl;
    if(localIndex == GL_INVALID_INDEX) {
      std::cout << this->name() << ": doesn't exist in shader" << endl;
    }

    glUniformBlockBinding(prog,localIndex,this->bindPoint());
    debugout << "glUniformBlockBinding(" << prog << ", " << localIndex;
    debugout << ", " << this->bindPoint() << ");" << endl;

    int sz;
    glGetActiveUniformBlockiv(prog, localIndex,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &sz);
    debugout << "uniform size: " << sz << endl;
  } else {
    std::map<std::string,GLint> locations;
    for(auto varp = this->types.begin(); varp != this->types.end(); varp++) {
      const auto varName = varp->first;
      GLint loc = glGetUniformLocation(prog, varName.c_str());
      locations.insert(std::make_pair(varName,loc));
    }
    this->basicLocations.insert(std::make_pair(prog,locations));
  }
}

void frameUpdate(shaderProgram* p) {
  localUniformUpdate(basicLocations[p][varName],1,false,data.data());
}

std::string gx::uniform::block::name() const {
  return this->blockName;
}

GLuint gx::uniform::block::bindPoint() const {
  return this->bindingIndex;
}
