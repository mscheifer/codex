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
        attr.stride(), nullptr);
      debugout << "glVertexAttribIPointer(" << this->shaderLocation << ", ";
      debugout << attr.vecSize() << ", " << typeToString(attr.type()) << ", ";
      debugout << attr.stride() << ", nullptr);" << endl;
    } else {
      glVertexAttribPointer(this->shaderLocation, attr.vecSize(), attr.type(),
        GL_FALSE, attr.stride(), nullptr);
      debugout << "glVertexAttribPointer(" << this->shaderLocation << ", ";
      debugout << attr.vecSize() << ", " << typeToString(attr.type());
      debugout << ", GL_FALSE, " << attr.stride() << ", nullptr);" << endl;
    }
    return true;
  } else {
    std::cout << this->components << " != " << attr.vecSize() << " || ";
    std::cout << this->glType     << " != " << attr.type()    << std::endl;
    return false;
  }
}
