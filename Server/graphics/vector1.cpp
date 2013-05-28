#include "vector1.h"

template<typename T>
constexpr unsigned int gx::vector1<T>::numComponents;

template<typename T>
gx::vector1<T>::vector1() {}

template<typename T>
gx::vector1<T>::vector1(T a) {
  this->elem[0] = std::move(a);
}

template<typename T>
typename std::array<T,1>::iterator gx::vector1<T>::begin() {
  return this->elem.begin();
}

template<typename T>
typename std::array<T,1>::iterator gx::vector1<T>::end() {
  return this->elem.end();
}

template class gx::vector1<GLfloat>;
