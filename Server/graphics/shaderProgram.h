#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include "oglUtil.h"
#include "vertexAttrib.h"
#include "vertexAttribSignature.h"

namespace gx {
namespace uniform {
class block;
} //end namspace uniform

class shaderProgram {
    GLuint prog;
    varSigs_t attribSigs;
  public:
    shaderProgram(const std::string,const std::string,
                  const std::vector<uniform::block*>);
    shaderProgram(const shaderProgram&);// = delete; //don't copy
    shaderProgram& operator=(const shaderProgram&);// = delete; //don't assign
    shaderProgram(shaderProgram&&);
    shaderProgram& operator=(shaderProgram&&);
    ~shaderProgram(); //not virtual because there's no inheiritance
    void use() const;
    varSigs_t vars() const;
    GLint uniformLoc(const std::string) const;
    GLuint progNum() const;
};

} //end namespace gx
#endif //SHADERPROGRAM_H
