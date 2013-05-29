#ifndef VECTOR1_H
#define VECTOR1_H
#include <GL/glew.h>
#include <array>
#include "windowsHacks.h"

namespace gx {
template<typename T>
class vector1 {
    std::array<T,1> elem;
  public:
    static constexpr unsigned int numComponents = 1;
    typedef T elem_t;
    vector1();
    vector1(T);
    typename std::array<T,1>::iterator begin();
    typename std::array<T,1>::iterator end();
};

typedef vector1<GLfloat> vector1f;
} //end namespace gx
#endif //VECTOR1_H
