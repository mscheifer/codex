#ifndef DISPLAY_SET_H
#define DISPLAY_SET_H
#include <GL/glew.h>
#include "oglUtil.h"
#include "matrix.h"
#include "uniformBlock.h"

namespace gx {

class displaySet {
    matrix         view;
    matrix         projection;
    vector4f       cameraPos;
    uniform::block unif;
  public:
    typedef matrix::elem_t elem_t;
    displaySet();
    void setProjection(elem_t fov, elem_t ratio, elem_t nearP, elem_t farP);
    //camera position, camera look at, up vector
    void setView(const vector4f&, const vector4f&, const vector3f&);
    void addView(const vector3f&, const vector3f&, const vector3f&);
    uniform::block& storage();
};

} //end namespace gx
#endif //DISPLAY_SET_H
