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
struct vao {
    typedef vertexAttrib::attribsList_t attribsList_t;
private:
        GLuint id;
       GLsizei numIndices;
        GLuint ibo;
    attribsList_t attribs;
  public:
    vao(const std::vector<GLuint>,attribsList_t,varSigs_t);
    vao(const vao&);// = delete; //don't copy
    vao& operator=(const vao&);// = delete; //don't assign
    vao(vao&&);
    vao& operator=(vao&&);
    ~vao();
    void drawOnce() const;
    void drawHead() const;
    void drawInstance() const;
};

} //end namespace gx
#endif
