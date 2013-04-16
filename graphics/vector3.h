#ifndef VECTOR3_H
#define VECTOR3_H
#include <GL/glew.h>
#include <iostream>
#include <cmath>

namespace gx {

class vector3 {
public:
  typedef GLfloat elem_t;
  elem_t x,y,z;
  vector3();
  vector3(elem_t,elem_t,elem_t);
  void set(elem_t,elem_t,elem_t);
  void add(const vector3&);
  void add(const vector3&,const vector3&);
  void subtract(const vector3&);
  void subtract(const vector3&,const vector3&);
  void negate();
  void negate(const vector3&);
  void scale(elem_t);
  void scale(elem_t,const vector3&);
  elem_t dot(const vector3&) const;
  void cross(const vector3&,const vector3&);
  elem_t magnitude() const;
  void normalize();

  elem_t& get(int);
  const elem_t& get(int) const;
  elem_t& operator[](int);
  const elem_t& operator[](int) const;
  vector3 operator-() const;
  vector3 operator+(const vector3&) const;
  vector3 operator-(const vector3&) const;
  vector3 operator*(const vector3 &a) const;
  vector3 operator*(elem_t f) const;
  void print(std::ostream& o) const;
  void print() const;
  bool operator==(const vector3& o) const;
  bool operator!=(const vector3& o) const;
};

vector3 operator*(vector3::elem_t, const vector3&);
std::ostream& operator<< (std::ostream&, const vector3&);

} //end namespace gx
#endif
