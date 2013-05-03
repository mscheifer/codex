#ifndef GLOBAL_UNIFORM_H
#define GLOBAL_UNIFORM_H
#include <GL/glew.h>
#include <string>
#include <array>
#include <map>
#include <vector>
#include <memory>
#include "basicUniform.h"

namespace gx {
class shaderProgram;

namespace uniform {
class block {
    static GLuint nextUniformBindPoint;
    static GLuint freshBindPoint();
    std::string blockName;
    GLuint      bindingIndex;
    GLuint      bufferName;

    std::map<std::string,GLintptr>      offsets;
	  std::vector<std::unique_ptr<basic>> basicStorage;
    std::map<std::string,unsigned int>  storageNums;
    void writeBuffer(GLintptr,GLsizeiptr,const GLvoid*) const;
    template<typename T>
    void writeBufferStruct(GLintptr offset,const T& data) const {
      if(gx::sharedUniformsOn) {
        this->write(offset, sizeof(data), &data);
      }
    }
  public:
    block(std::string,std::map<std::string,GLenum>);
    block(const block&);// = delete;
    block& operator=(const block&);// = delete;
    block(block&&);// = delete;
    block& operator=(block&&);// = delete;
    ~block();
    void addShaderBindings(shaderProgram*);
    template<typename T,size_t N>
    void writeBuffer(GLintptr offset,const std::array<T,N>& data) const {
      if(gx::sharedUniformsOn) {
        this->writeBuffer(offset, sizeof(data), data.data());
      }
    }
    template<typename T,size_t N>
    void writeNum(unsigned int num, const std::array<T,N>& data) {
      if(!gx::sharedUniformsOn) {
        this->basicStorage[num]->write(data.data());
      }
    }
    template<typename T,size_t N>
    void write(const std::string varName, const std::array<T,N>& data) {
      this->writeBuffer(this->offsets[varName],std::forward<const std::array<T,N>>(data));
      this->writeNum(this->storageNums[varName],std::forward<const std::array<T,N>>(data));
    }
    void frameUpdate(const shaderProgram*) const;

    std::string declaration() const;
    std::string name()        const;
    GLuint      bindPoint()   const;
};
} //end namespace uniform
} //end namepace gx
#endif //GLOBAL_UNIFORM_H
