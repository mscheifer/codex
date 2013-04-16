#ifndef VAO_H
#define VAO_H
#include <GL/glew.h>
#include <vector>
#include "vertexAttrib.h"

namespace gx {

class vao {
    GLuint  id;
    GLsizei numIndices;
  public:
    vao(std::vector<GLuint>,std::vector<vertexAttrib>);
    void draw();
};

} //end namespace gx
#endif
