#ifndef UNIFORM_H
#define UNIFORM_H
#include <GL/glew.h>
#include <string>
#include <array>

namespace gx {

class uniform {
    static GLuint nextUniformBindPoint;
    static GLuint freshBindPoint();
    std::string varName;
    GLuint      bindingIndex;
    GLuint      bufferName;
  public:
    uniform(std::string,GLsizeiptr);
    uniform(const uniform&);// = delete;
    uniform& operator=(const uniform&);// = delete;
    uniform(uniform&&);// = delete;
    uniform& operator=(uniform&&);// = delete;
    ~uniform();
    void write(GLintptr,GLsizeiptr,const GLvoid*) const;
    template<typename T,size_t N>
    void write(GLintptr offset,const std::array<T,N>& data) const {
      this->write(offset, sizeof(data), data.data());
    }
    template<typename T>
    void writeStruct(GLintptr offset,const T& data) const {
      this->write(offset, sizeof(data), &data);
    }
    std::string name()      const;
    GLuint      bindPoint() const;
};

} //end namepace gx
#endif //UNIFORM_H
