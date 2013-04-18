#include <GL/glew.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "oglUtil.h"
#include "matrix.h"

namespace gx {

class displaySet {
    static GLuint nextUniformBindPoint;
    static GLuint freshBindPoint();
    matrix view;
    matrix projection;
    GLuint bindingIndex;
    GLuint bufferName;
  public:
    typedef matrix::elem_t elem_t;
    displaySet();
    ~displaySet();
    void setProjection(elem_t fov, elem_t ratio, elem_t nearP, elem_t farP);
    //camera position, camera look at, up vector
    void setView(const vector3&, const vector3&, const vector3&);
    GLuint bindPoint();
};

} //end namespace gx
