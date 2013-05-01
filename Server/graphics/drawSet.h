#include <GL/glew.h>
#include <vector>
#include <string>
#include "matrix.h"
#include "uniform.h"
#include "shaderProgram.h"
#include "vao.h"

namespace gx {

class drawSet {
   struct entityClass {
      std::vector<matrix> positions;
      vao                 vertData;
      entityClass(std::vector<matrix>,vao);
      entityClass(entityClass&&);
    };
    shaderProgram            program;
    std::vector<entityClass> entityClasses;
    GLint                    modelToWorldLoc;
  public:
    typedef std::pair<std::vector<GLuint>,
                      std::vector<const vertexAttrib*>> vaoData_t;
    drawSet(const std::string, const std::string, const std::vector<vaoData_t>,
            std::vector<const uniform*>);
    void draw(matrix) const;
    void reset();
    void addEntity(vector3,unsigned int);
};

} //end namespace gx
