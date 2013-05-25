#ifndef VERTXATTRIBSIGNATURE_H
#define VERTXATTRIBSIGNATURE_H
#include <GL/glew.h>
#include <map>

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

typedef std::map<std::string,vertexAttribSignature> varSigs_t;

}//end namespace gx
#endif //VERTXATTRIBSIGNATURE_H
