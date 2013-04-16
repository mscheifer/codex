#include <GL/glew.h>
#include "oglUtil.h"
#include "matrix.h"

namespace gx {

class displaySet {
  public:
    matrix view;
    matrix projection;
    GLuint bindingIndex;
    GLuint bufferName;
  public:
    typedef matrix::elem_t elem_t;
    displaySet(GLuint);
    void setProjection(elem_t fov, elem_t ratio, elem_t nearP, elem_t farP);
    //camera position, camera look at, up vector
    void setView(const vector3&, const vector3&, const vector3&);
    GLuint bindPoint();
};

} //end namespace gx
