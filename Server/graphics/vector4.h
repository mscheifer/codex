#ifndef VECTOR4_H
#define VECTOR4_H
#include <iostream>
#include "oglUtil.h"

namespace gx {
template<typename T>
class vector3;

typedef vector3<GLfloat> vector3f;

class vector4 {
  template<class Ret, class C>
  static Ret& index(C& a, int i)
          {return (i == 0) ? a.x : (i == 1) ? a.y : (i == 2) ? a.z : a.w;}
public:
  typedef float elem_t;
  elem_t x,y,z,w;
  vector4();
  vector4(elem_t,elem_t,elem_t);
  vector4(const std::initializer_list<elem_t>);
  void set(elem_t,elem_t,elem_t,elem_t);
  void add(const vector3f&);
  void add(const vector4&,const vector3f&);
  void subtract(vector4&);
  void subtract(vector4&,vector4&);
  void dehomogenize();

  elem_t& get(int);
  const elem_t& get(int i) const;
  elem_t& operator[](int i); 
  const elem_t& operator[](int i) const;
  vector4 operator+(const vector3f&) const;
  vector3f operator-(vector4) const;      
  vector4& operator+=(const vector3f&);
  bool operator==(const vector4&) const;
  bool operator!=(const vector4&) const;
  void print(std::ostream&) const;
  void print() const;
};

vector4 dehomogenize(vector4);

std::ostream& operator<< (std::ostream& out, const vector4& v);

} //end namespace gx
#endif
