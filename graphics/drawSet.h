#include <GL/glew.h>
#include <vector>
#include <string>
#include "matrix.h"
#include "uniform.h"
#include "shaderProgram.h"
#include "vao.h"

namespace gx {

struct entityClass {
  std::vector<matrix> positions;
  vao                 vertData;
};

class drawSet {
    uniform                  instancePos; //add another for direction
    shaderProgram            program;
    std::vector<entityClass> entityClasses;
  public:
    typedef std::pair<std::vector<GLuint>,
                      std::vector<const vertexAttrib*>> vaoData_t;
    drawSet(const std::string, const std::string, const std::vector<vaoData_t>,
            std::vector<const uniform*>);
    void draw() const;
    void addEntity(vector3,unsigned int);
};

} //end namespace gx
