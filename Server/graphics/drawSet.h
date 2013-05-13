#ifndef DRAWSET_H
#define DRAWSET_H
#include <GL/glew.h>
#include <vector>
#include <string>
#include "shaderProgram.h"
#include "vao.h"
#include "matrix.h"
#include "uniformLocation.h"

namespace gx {
class displaySet;
namespace uniform {
class block;
} //end namespace uniform

class drawSet {
   struct entityClass {
      std::vector<matrix> positions;
      vao                 vertData;
      entityClass(std::vector<matrix>,vao);
      entityClass(entityClass&&);
    };
    shaderProgram            program;
    std::vector<entityClass> entityClasses;
    uniform::mat4floc        modelToWorldLoc;
	  std::vector<uniform::block*> globalUniforms;
  public:
    typedef std::pair<std::vector<GLuint>,
                      std::vector<const vertexAttrib*>> vaoData_t;
    drawSet(const std::string, const std::string, const std::vector<vaoData_t>,
                  std::vector<uniform::block*>);
    void draw() const;
    void reset();
    void addEntity(vector4f,vector3f,unsigned int);
};

} //end namespace gx
#endif // DRAWSET_H
