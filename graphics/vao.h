#ifndef VAO_H
#define VAO_H
#include <GL/glew.h>
#include <vector>
#include "vertexAttrib.h"

namespace gx {

class vao {
    GLuint  id;
    GLsizei numIndices;
    GLuint ibo;
  public:
    vao(std::vector<GLuint>,std::vector<const vertexAttrib*>);
    ~vao();
    void draw();
};

} //end namespace gx
#endif
