#include "vertexAttribSignature.h"
#include "vertexAttrib.h"

gx::vertexAttribSignature::vertexAttribSignature(GLenum type, GLuint loc)
: components(typeComponents(type)),glType(baseType(type)),shaderLocation(loc) {}

bool gx::vertexAttribSignature::checkAndBind(const vertexAttrib& attr) {
  if(this->components == attr.vecSize() && this->glType == attr.type()) {
    attr.bindBuffer();
    glEnableVertexAttribArray(this->shaderLocation);
    debugout << "glEnableVertexAttribArray(" << this->shaderLocation;
    debugout << ");" << endl;
    if(this->glType == GL_INT   || this->glType == GL_UNSIGNED_INT ||
       this->glType == GL_SHORT                                    ) {
      glVertexAttribIPointer(this->shaderLocation, attr.vecSize(), attr.type(),
        attr.stride(), 0);
      debugout << "glVertexAttribIPointer(" << this->shaderLocation << ", ";
      debugout << attr.vecSize() << ", " << typeToString(attr.type()) << ", ";
      debugout << attr.stride() << ", 0);" << endl;
    } else {
      glVertexAttribPointer(this->shaderLocation, attr.vecSize(), attr.type(),
        GL_FALSE, attr.stride(), 0);
      debugout << "glVertexAttribPointer(" << this->shaderLocation << ", ";
      debugout << attr.vecSize() << ", " << typeToString(attr.type());
      debugout << ", GL_FALSE, " << attr.stride() << ", 0);" << endl;
    }
    return true;
  } else {
    return false;
  }
}
