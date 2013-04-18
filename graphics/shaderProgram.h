#include <GL/glew.h>
#include <string>
#include <vector>
#include <utility>
#include "oglUtil.h"
#include "vertexAttrib.h"

namespace gx {

class shaderProgram {
    GLuint prog;
  public:
    shaderProgram(std::string,std::string,
                  std::vector<std::pair<const std::string,GLuint>>,
                  std::vector<const vertexAttrib*>);
	//vc++ is dumb
    //shaderProgram(const shaderProgram&) = delete; //don't copy
    //shaderProgram& operator=(const shaderProgram&) = delete; //don't assign
    shaderProgram(shaderProgram&&);
    shaderProgram& operator=(shaderProgram&&);
    ~shaderProgram(); //not virtual because there's no inheiritance
    void use();
};

shaderProgram shaderFromFiles(const std::string,const std::string,
        std::vector<std::pair<const std::string,GLuint>>,
        std::vector<const vertexAttrib*>);

} //end namespace gx
