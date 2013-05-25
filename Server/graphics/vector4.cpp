#include "vector4.h"
#include "vector3.h"

template<typename T>
gx::vector4<T>::vector4(): x(elems[0]), y(elems[1]), z(elems[2]), w(elems[3]) {
  this->elems[0] = 0; this->elems[1] = 0; this->elems[2] = 0; this->elems[3]= 1;
}
template<typename T>
gx::vector4<T>::vector4(const vector4<T>& other)
  : x(elems[0]), y(elems[1]), z(elems[2]), w(elems[3]) {
  this->elems = other.elems;
}
template<typename T>
gx::vector4<T>::vector4(vector4<T>&& other)
  : x(elems[0]), y(elems[1]), z(elems[2]), w(elems[3]) {
  this->elems = std::move(other.elems);
}
template<typename T>
gx::vector4<T>& gx::vector4<T>::operator=(const vector4<T>& other) {
  this->elems = other.elems;
  return *this;
}
template<typename T>
gx::vector4<T>& gx::vector4<T>::operator=(vector4<T>&& other) {
  this->elems = std::move(other.elems);
  return *this;
}
template<typename T>
gx::vector4<T>::vector4(elem_t x0, elem_t y0, elem_t z0)
  : x(elems[0]), y(elems[1]), z(elems[2]), w(elems[3]) {
  this->elems[0]= x0; this->elems[1]= y0; this->elems[2]= z0; this->elems[3]= 1;
}
template<typename T>
gx::vector4<T>::vector4(const std::initializer_list<elem_t> l)
  : x(elems[0]), y(elems[1]), z(elems[2]), w(elems[3]) {
  auto it = elems.begin();
  for(auto itl = l.begin(); it != elems.end() && itl != l.end(); it++, itl++) {
    *it = *itl;
  }
}
template<typename T>
void gx::vector4<T>::set(elem_t x0, elem_t y0, elem_t z0, elem_t w0) {
  this->x = x0;
  this->y = y0;
  this->z = z0;
  this->w = w0;
}
template<typename T>
void gx::vector4<T>::add(const vector3<T>& a) {
  this->x += a.x;
  this->y += a.y;
  this->z += a.z;
}
template<typename T>
void gx::vector4<T>::add(const vector4<T>& a,const vector3<T>& b) {
  this->x = a.x + b.x;
  this->y = a.y + b.y;
  this->z = a.z + b.z;
  this->w = 1;
}
template<typename T>
void gx::vector4<T>::subtract(const vector3<T>& a) {
  this->x -= a.x;
  this->y -= a.y;
  this->z -= a.z;
}
template<typename T>
void gx::vector4<T>::subtract(const vector4<T>& a,const vector3<T>& b) {
  this->x = a.x - b.x;
  this->y = a.y - b.y;
  this->z = a.z - b.z;
  this->w = a.w;
}
template<typename T>
void gx::vector4<T>::dehomogenize() {
  this->x = this->x / this->w;
  this->y = this->y / this->w;
  this->z = this->z / this->w;
  this->w = 1;
}
template<typename T>
typename gx::vector4<T>::elem_t& gx::vector4<T>::get(index_type i) {
  return this->elems[i];
}  
template<typename T>
const typename gx::vector4<T>::elem_t& gx::vector4<T>::get(index_type i) const {
  return this->elems[i];
}
template<typename T>
typename gx::vector4<T>::elem_t& gx::vector4<T>::operator[](index_type i) {
  return this->get(i);
}
template<typename T>
const typename gx::vector4<T>::elem_t& gx::vector4<T>::operator[](index_type i) const {
  return this->get(i);
}
template<typename T>
gx::vector4<T> gx::vector4<T>::operator+(const vector3<T>& a) const {
  vector4 r;
  r.add(*this,a);
  return r;
}
template<typename T>
gx::vector3<T> gx::vector4<T>::operator-(vector4<T> a) const {
  a.dehomogenize();
  vector4<T> r = *this;
  r.dehomogenize();
  r.subtract(vector3<T>(a.x,a.y,a.z));
  return vector3<T>(r.x,r.y,r.z);
}
template<typename T>
gx::vector4<T>& gx::vector4<T>::operator+=(const vector3<T>& a) {
  this->add(a);
  return *this;
}
template<typename T>
bool gx::vector4<T>::operator==(const vector4<T>& a) const {
  //think about a better way to do this with floating point types
  return this->x == a.x && this->y == a.y && this->z == a.z && this->w == a.w;
}
template<typename T>
bool gx::vector4<T>::operator!=(const vector4<T>& a) const {
  return !(*this == a);
}
template<typename T>
void gx::vector4<T>::print(std::ostream& o) const {
  o << this->x << " " << this->y << " " << this->z  << " " << this->w;
}
template<typename T>
void gx::vector4<T>::print() const {
  this->print(std::cout);
}

template<typename T>
gx::vector4<T> gx::dehomogenize(gx::vector4<T> a) {
  a.dehomogenize();
  return a;
}

template<typename T>
std::ostream& gx::operator<< (std::ostream& out, const vector4<T>& v) {
  v.print(out);
  return out;
}

template class gx::vector4<GLfloat>;
template gx::vector4<GLfloat> gx::dehomogenize<GLfloat>(vector4<GLfloat>);
template std::ostream&
gx::operator<<<GLfloat> (std::ostream&, const vector4<GLfloat>&);
