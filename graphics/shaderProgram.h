#include <GL/glew.h>
#include <string>
#include <vector>
#include <utility>
#include "oglUtil.h"

namespace gx {

class shaderProgram {
  public:
    GLuint prog;
  public:
    shaderProgram(std::string,std::string,
            const std::vector<std::pair<const std::string,GLuint>>,
            const std::vector<std::pair<const std::string,GLuint>>);
	shaderProgram(shaderProgram&&);
    ~shaderProgram(); //not virtual because there's no inheiritance
    void use();
};

shaderProgram shaderFromFiles(const std::string,const std::string,
        const std::vector<std::pair<const std::string,GLuint>>,
        const std::vector<std::pair<const std::string,GLuint>>);

} //end namespace gx
