#include "vector2.h"

template<typename T>
constexpr unsigned int gx::vector2<T>::numComponents;

template<typename T>
gx::vector2<T>::vector2() {}

template<typename T>
gx::vector2<T>::vector2(elem_t a, elem_t b) {
  this->elems[0] = std::move(a);
  this->elems[1] = std::move(b);
}

template<typename T>
typename std::array<T,2>::iterator gx::vector2<T>::begin() {
  return this->elems.begin();
}

template<typename T>
typename std::array<T,2>::iterator gx::vector2<T>::end() {
  return this->elems.end();
}

template class gx::vector2<GLfloat>;