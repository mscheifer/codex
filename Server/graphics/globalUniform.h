#ifndef GLOBAL_UNIFORM_H
#define GLOBAL_UNIFORM_H
#include <GL/glew.h>
#include <string>
#include <array>
#include <map>

namespace { //to not export
  template<GLenum T>
  void localUniformUpdate(GLint, const GLfloat*) {
    std::cout << "error for" << typeVal(T);
  }
  template<>
  void localUniformUpdate<GL_FLOAT_VEC4>(GLint location, const GLfloat* value) {
    //just do 1 for now. handle arrays later
    glUniform4fv(location,1,value);
  }
  template<>
  void localUniformUpdate<GL_FLOAT_MAT4>(GLint location, const GLfloat* value) {
    glUniformMatrix4fv(location,1,value);
  }
} //end unnamed namespace

namespace gx {

class shaderProgram;

class globalUniform {
    static GLuint nextUniformBindPoint;
    static GLuint freshBindPoint();
    std::string blockName;
    GLuint      bindingIndex;
    GLuint      bufferName;
	  std::map<std::string,GLenum> types;
	  std::map<std::string,GLintptr> offsets;
    //map of shaderPrograms to a map of var names to locations
	  std::map<shaderProgram*,std::map<std::string,GLint>> basicLocations;
    void writeBuffer(GLintptr,GLsizeiptr,const GLvoid*) const;
    template<typename T,size_t N>
    void writeBuffer(GLintptr offset,const std::array<T,N>& data) const {
      this->write(offset, sizeof(data), data.data());
    }
    template<typename T>
    void writeBufferStruct(GLintptr offset,const T& data) const {
      this->write(offset, sizeof(data), &data);
    }
  public:
    globalUniform(std::string,std::map<std::string,GLenum>);
    globalUniform(const globalUniform&);// = delete;
    globalUniform& operator=(const globalUniform&);// = delete;
    globalUniform(globalUniform&&);// = delete;
    globalUniform& operator=(globalUniform&&);// = delete;
    ~globalUniform();
    void addShaderBindings(shaderProgram*);
    template<typename T,size_t N, GLenum type>
    void write(const std::string varName,const, const std::array<T,N>& data) {
      if(type != this->types[varName]) {
        std::cout << "Error bad type in uniform update for" << varName << std::endl;
      }
      if(gx::sharedUniformsOn) {
        this->writeBuffer(this->offsets[varName],std::forward<std::array<T,N>(data));
      } else {
        for(auto shadp = basicLocations.begin(); shadp != basicLocations.end(); shadp++) {
          shadp->first->use();
          localUniformUpdate(shadp->second[varName],1,false,data.data());
        }
      }
    }
    std::string name()      const;
    GLuint      bindPoint() const;
};

} //end namepace gx
#endif //GLOBAL_UNIFORM_H
