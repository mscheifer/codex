#ifndef DISPLAY_SET_H
#define DISPLAY_SET_H
#include <GL/glew.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "oglUtil.h"
#include "matrix.h"
#include "uniform.h"

namespace gx {

class displaySet {
    matrix  view;
    matrix  projection;
    vector3 cameraPos;
    uniform unif;
  public:
    typedef matrix::elem_t elem_t;
    displaySet();
    void setProjection(elem_t fov, elem_t ratio, elem_t nearP, elem_t farP);
    //camera position, camera look at, up vector
    void setView(const vector3&, const vector3&, const vector3&);
    void addView(const vector3&, const vector3&, const vector3&);
    const uniform& storage() const;
};

} //end namespace gx
#endif
