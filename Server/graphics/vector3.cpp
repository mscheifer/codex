#include "vector3.h"
#include <cmath>

template<typename T>
gx::vector3<T>::vector3()
  : x(elems[0]), y(elems[1]), z(elems[2]) {
  elems[0] = 0; elems[1] = 0; elems[2] = 0;
}
template<typename T>
gx::vector3<T>::vector3(elem_t x0, elem_t y0, elem_t z0)
  : x(elems[0]), y(elems[1]), z(elems[2]) {
  this->x = x0;
  this->y = y0;
  this->z = z0;
}
template<typename T>
gx::vector3<T>::vector3(const vector3<T>& other)
  : elems(other.elems), x(elems[0]), y(elems[1]), z(elems[2]) {}

template<typename T>
gx::vector3<T>::vector3(vector3<T>&& other)
  : elems(std::move(other.elems)), x(elems[0]), y(elems[1]), z(elems[2]) {}

template<typename T>
gx::vector3<T>& gx::vector3<T>::operator=(vector3<T>&& other) {
  this->elems = std::move(other.elems);
  return *this;
}
template<typename T>
gx::vector3<T>& gx::vector3<T>::operator=(const vector3<T>& other) {
  this->elems = other.elems;
  return *this;
}
template<typename T>
void gx::vector3<T>::set(elem_t x0, elem_t y0, elem_t z0) {
  this->x = x0;
  this->y = y0;
  this->z = z0;
}
template<typename T>
void gx::vector3<T>::add(const vector3<T> &a) {
  this->x += a.x;
  this->y += a.y;
  this->z += a.z;
}
template<typename T>
void gx::vector3<T>::add(const vector3<T>& a, const vector3<T>& b) {
  this->x = a.x + b.x;
  this->y = a.y + b.y;
  this->z = a.z + b.z;
}
template<typename T>
void gx::vector3<T>::subtract(const vector3<T> &a) {
  this->x -= a.x;
  this->y -= a.y;
  this->z -= a.z;
}
template<typename T>
void gx::vector3<T>::subtract(const vector3<T> &a,const vector3<T> &b) {
  this->x = a.x - b.x;
  this->y = a.y - b.y;
  this->z = a.z - b.z;
}
template<typename T>
void gx::vector3<T>::negate() {
  this->x = -x;
  this->y = -y;
  this->z = -z;
}
template<typename T>
void gx::vector3<T>::negate(const vector3<T> &a) {
  this->x = -a.x;
  this->y = -a.y;
  this->z = -a.z;
}
template<typename T>
void gx::vector3<T>::scale(elem_t s) {
  this->x *= s;
  this->y *= s;
  this->z *= s;
}
template<typename T>
void gx::vector3<T>::scale(elem_t s,const vector3<T> &a) {
  this->x = s * a.x;
  this->y = s * a.y;
  this->z = s * a.z;
}
template<typename T>
typename gx::vector3<T>::elem_t gx::vector3<T>::dot(const vector3<T> &a) const {
  return this->x * a.x + this->y * a.y + this->z * a.z;
}
template<typename T>
void gx::vector3<T>::cross(const vector3<T> &a,const vector3<T> &b) {
  this->x = a.y * b.z - a.z * b.y;
  this->y = a.z * b.x - a.x * b.z;
  this->z = a.x * b.y - a.y * b.x;
}
template<typename T>
typename gx::vector3<T>::elem_t gx::vector3<T>::magnitudesq() const {
  return x*x + y*y + z*z;
}
template<typename T>
typename gx::vector3<T>::elem_t gx::vector3<T>::magnitude() const {
  return static_cast<elem_t>(sqrt(this->magnitudesq()));
}
template<typename T>
void gx::vector3<T>::normalize() {
  if(this->magnitude() > 0) {
    this->scale(static_cast<elem_t>(1)/this->magnitude());
  }
}
template<typename T>
std::array<typename gx::vector3<T>::elem_t,3> gx::vector3<T>::oglVec3() const {
  return elems;
}
template<typename T>
typename gx::vector3<T>::elem_t& gx::vector3<T>::get(int i) {
  return elems[i];
}
template<typename T>
const typename gx::vector3<T>::elem_t& gx::vector3<T>::get(int i) const {
  return elems[i];
}
template<typename T>
typename gx::vector3<T>::elem_t& gx::vector3<T>::operator[](int i) {
  return get(i);
}
template<typename T>
const typename gx::vector3<T>::elem_t& gx::vector3<T>::operator[](int i) const {
  return get(i);
}
template<typename T>
gx::vector3<T> gx::vector3<T>::operator-() const {
  vector3<T> r; 
  r.negate(*this);
  return r;
}
template<typename T>
gx::vector3<T> gx::vector3<T>::operator+(const vector3<T> &a) const {
  vector3<T> r;
  r.add(*this,a);
  return r;
}
template<typename T>
gx::vector4<T> gx::vector3<T>::operator+(const vector4<T> &a) const {
  return a + *this;
}
template<typename T>
gx::vector3<T> gx::vector3<T>::operator-(const vector3<T>& a) const {
  vector3<T> r;
  r.subtract(*this,a);
  return r;
}
template<typename T>
gx::vector3<T> gx::vector3<T>::operator*(const vector3<T> &a) const {
  vector3<T> r;
  r.cross(*this,a);
  return r;
}
template<typename T>
gx::vector3<T> gx::vector3<T>::operator*(elem_t f) const {
  vector3<T> r;
  r.scale(f,*this);
  return r;
}
template<typename T>
gx::vector3<T>& gx::vector3<T>::operator+=(const vector3<T>& o) {
  this->x = this->x + o.x;
  this->y = this->y + o.y;
  this->z = this->z + o.z;
  return *this;
}
template<typename T>
void gx::vector3<T>::print(std::ostream& o) const {
  o << this->x << " " << this->y << " " << this->z;
}
template<typename T>
void gx::vector3<T>::print() const {
  this->print(std::cout);
}
template<typename T>
bool gx::vector3<T>::operator==(const vector3<T>& o) const {
  return this->get(0) == o.get(0) && this->get(1) == o.get(1) &&
         this->get(2) == o.get(2);
}
template<typename T>
bool gx::vector3<T>::operator!=(const vector3<T>& o) const {
  return !((*this)==o);
}

template<typename T>
gx::vector3<T> gx::operator*(typename gx::vector3<T>::elem_t f, const gx::vector3<T>& v) {
  return v * f;
}

template<typename T>
std::ostream& gx::operator<< (std::ostream& out, const gx::vector3<T>& v) {
  v.print(out);
  return out;
}

template<typename T>
void gx::vector3<T>::serialize(sf::Packet & packet) const {
    packet<<x;
    packet<<y;
    packet<<z;
}

template<typename T>
void gx::vector3<T>::deserialize(sf::Packet & packet) {
    packet>>x;
    packet>>y;
    packet>>z;
}

template class gx::vector3<GLfloat>;
template gx::vector3<GLfloat> gx::operator*<GLfloat>(vector3<GLfloat>::elem_t, const vector3<GLfloat>&);
template std::ostream& gx::operator<<<GLfloat> (std::ostream&, const vector3<GLfloat>&);

//template class gx::vector3<double>;
//template gx::vector3<double> gx::operator*<double>(vector3<double>::elem_t, const vector3<double>&);
//template std::ostream& gx::operator<<<double> (std::ostream& out, const vector3<double>& v);
