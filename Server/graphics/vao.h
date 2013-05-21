#ifndef VAO_H
#define VAO_H
#include <GL/glew.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "vertexAttrib.h"
#include "vertexAttribSignature.h"

namespace gx {
class vao {
    typedef vertexAttrib::attribsList_t attribsList_t;
        GLuint id;
       GLsizei numIndices;
        GLuint ibo;
    attribsList_t attribs;
  public:
    vao(const std::vector<GLuint>,attribsList_t,std::map<std::string,vertexAttribSignature>);
    vao(const vao&);// = delete; //don't copy
    vao& operator=(const vao&);// = delete; //don't assign
    vao(vao&&);
    vao& operator=(vao&&);
    ~vao();
    void draw() const;
};

} //end namespace gx
#endif
