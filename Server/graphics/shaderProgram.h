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

class uniform;

class shaderProgram {
    GLuint prog;
    std::map<std::string,vertexAttribSignature> attribSigs;
  public:
    shaderProgram(const std::string,const std::string,
                  const std::vector<const uniform*>);
	//vc++ is dumb
    //shaderProgram(const shaderProgram&) = delete; //don't copy
    //shaderProgram& operator=(const shaderProgram&) = delete; //don't assign
    shaderProgram(shaderProgram&&);
    shaderProgram& operator=(shaderProgram&&);
    ~shaderProgram(); //not virtual because there's no inheiritance
    void use() const;
    std::map<std::string,vertexAttribSignature> vars() const;
    GLint uniformLoc(const std::string) const;
};

} //end namespace gx
#endif //SHADERPROGRAM_H
