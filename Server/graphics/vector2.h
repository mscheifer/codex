#ifndef VECTOR2_H
#define VECTOR2_H
#include <GL/glew.h>
#include <array>
#include "windowsHacks.h"

namespace gx {
template<typename T>
class vector2 {
    std::array<T,2> elems;
  public:
    static constexpr unsigned int numComponents = 2;
    typedef T elem_t;
    vector2();
    vector2(elem_t,elem_t);
    typename std::array<T,2>::iterator begin();
    typename std::array<T,2>::iterator end();
};

typedef vector2<GLfloat> vector2f;
} //end namespace gx
#endif //VECTOR2_H