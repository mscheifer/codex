#ifndef VAO_H
#define VAO_H
#include <GL/glew.h>
#include <vector>
#include <map>
#include <string>
#include "vertexAttribSignature.h"

namespace gx {
class vertexAttrib;

class vao {
    GLuint  id;
    GLsizei numIndices;
    GLuint  ibo;
  public:
    vao(const std::vector<GLuint>,const std::vector<const vertexAttrib*>,
        std::map<std::string,vertexAttribSignature>);
    vao(const vao&);// = delete; //don't copy
    vao& operator=(const vao&);// = delete; //don't assign
    vao(vao&&);
    vao& operator=(vao&&);
    ~vao();
    void draw() const;
};

} //end namespace gx
#endif
