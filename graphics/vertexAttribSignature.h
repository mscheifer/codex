#ifndef VERTXATTRIBSIGNATURE_H
#define VERTXATTRIBSIGNATURE_H
#include <GL/glew.h>

namespace gx {

class vertexAttrib;

class vertexAttribSignature {
    GLint  components;
    GLenum glType;
    GLuint shaderLocation;
  public:
    vertexAttribSignature(GLenum, GLuint);
    bool checkAndBind(const vertexAttrib&);
};

}//end namespace gx
#endif //VERTXATTRIBSIGNATURE_H
