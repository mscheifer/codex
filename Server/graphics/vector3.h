#ifndef VECTOR3_H
#define VECTOR3_H
#include <GL/glew.h>
#include <iostream>
#include <cmath>
#include <array>
#include "vector4.h"

namespace gx {

template<typename T>
class vector3 {
    std::array<T,3> elems;
  public:
    typedef T elem_t;
    elem_t& x;
    elem_t& y;
    elem_t& z;
    vector3(): x(elems[0]), y(elems[1]), z(elems[2]) {
      elems[0] = 0; elems[1] = 0; elems[2] = 0;
	}
    vector3(elem_t x0,elem_t y0,elem_t z0): x(elems[0]), y(elems[1]), z(elems[2]) {
      this->x = x0;
      this->y = y0;
      this->z = z0;
	}
    vector3<T>& operator=(const vector3<T>& other) {
      this->elems = other.elems;
      return *this;
    }
    void set(elem_t x0, elem_t y0, elem_t z0) {
      this->x = x0;
      this->y = y0;
      this->z = z0;
    }
    void add(const vector3<T> &a) {
      this->x += a.x;
      this->y += a.y;
      this->z += a.z;
    }
    void add(const vector3<T> &a,const vector3<T> &b) {
      this->x = a.x + b.x;
      this->y = a.y + b.y;
      this->z = a.z + b.z;
    }
    void subtract(const vector3<T> &a) {
      this->x -= a.x;
      this->y -= a.y;
      this->z -= a.z;
    }
    void subtract(const vector3<T> &a,const vector3<T> &b) {
      this->x = a.x - b.x;
      this->y = a.y - b.y;
      this->z = a.z - b.z;
    }
    void negate() {
      this->x = -x;
      this->y = -y;
      this->z = -z;
    }
    void negate(const vector3<T> &a) {
      this->x = -a.x;
      this->y = -a.y;
      this->z = -a.z;
    }
    void scale(elem_t s) {
      this->x *= s;
      this->y *= s;
      this->z *= s;
    }
    void scale(elem_t s,const vector3<T> &a) {
      this->x = s * a.x;
      this->y = s * a.y;
      this->z = s * a.z;
    }
    elem_t dot(const vector3<T> &a) const {
      return this->x * a.x + this->y * a.y + this->z * a.z;
    }
    void cross(const vector3<T> &a,const vector3<T> &b) {
      this->x = a.y * b.z - a.z * b.y;
      this->y = a.z * b.x - a.x * b.z;
      this->z = a.x * b.y - a.y * b.x;
    }
    elem_t magnitudesq() const {
      return x*x + y*y + z*z;
    }
    elem_t magnitude() const {
      return static_cast<elem_t>(sqrt(this->magnitudesq()));
    }
    void normalize() {
      if(this->magnitude() > 0) {
        this->scale(static_cast<elem_t>(1.0)/this->magnitude());
      }
    }
    std::array<elem_t,3> oglVec3() const {
      return elems;
    }
    elem_t& get(int i) {
      return elems[i];
    }
    const elem_t& get(int i) const {
      return elems[i];
    }
    elem_t& operator[](int i) {
      return get(i);
    }
    const elem_t& operator[](int i) const {
      return get(i);
    }
    vector3<T> operator-() const {
      vector3<T> r; 
      r.negate(*this);
      return r;
    }
    vector3<T> operator+(const vector3<T> &a) const {
      vector3<T> r;
      r.add(*this,a);
      return r;
    }
    vector4 operator+(const vector4 &a) const {
      return a + *this;
    }
    vector3<T> operator-(const vector3<T>& a) const {
      vector3<T> r;
      r.subtract(*this,a);
      return r;
    }
    vector3<T> operator*(const vector3<T> &a) const {
      vector3<T> r;
      r.cross(*this,a);
      return r;
    }
    vector3<T> operator*(elem_t f) const {
      vector3<T> r;
      r.scale(f,*this);
      return r;
    }
    vector3<T>& operator+=(const vector3<T>& o) {
      this->x = this->x + o.x;
      this->y = this->y + o.y;
      this->z = this->z + o.z;
      return *this;
    }
    void print(std::ostream& o) const {
      o << this->x << " " << this->y << " " << this->z;
    }
    void print() const {
      this->print(std::cout);
    }
    bool operator==(const vector3<T>& o) const {
      return this->get(0) == o.get(0) && this->get(1) == o.get(1) &&
             this->get(2) == o.get(2);
    }
    bool operator!=(const vector3<T>& o) const {
      return !((*this)==o);
    }
};

template<typename T>
vector3<T> operator*(typename vector3<T>::elem_t f, const vector3<T>& v) {
  return v * f;
}
template<typename T>
std::ostream& operator<< (std::ostream& out, const vector3<T>& v) {
  v.print(out);
  return out;
}

typedef vector3<GLfloat> vector3f;

} //end namespace gx
#endif
