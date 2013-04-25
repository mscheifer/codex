///////////////////////////////////////////////////////
//////////TODO: remove this file, it should be using the graphics vector4 ///////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


#ifndef VECTOR4_H
#define VECTOR4_H
#include <iostream>
#include "vector3.h"

namespace gx {

class vector4 {
  template<class Ret, class C>
  static Ret& index(C& a, int i)
          {return (i == 0) ? a.x : (i == 1) ? a.y : (i == 2) ? a.z : a.w;}
public:
  typedef float elem_t;
  elem_t x,y,z,w;
  vector4();
  vector4(elem_t,elem_t,elem_t);
  //vector4(const std::initializer_list<elem_t>);
  void set(elem_t,elem_t,elem_t,elem_t);
  void add(const vector3&);
  void add(const vector4&,const vector3&);
  void subtract(vector4&);
  void subtract(vector4&,vector4&);
  void dehomogenize();

  elem_t& get(int);
  const elem_t& get(int i) const;
  elem_t& operator[](int i); 
  const elem_t& operator[](int i) const;
  vector4 operator+(const vector3&) const;
  vector3 operator-(vector4) const;      
  bool operator==(const vector4&) const;
  bool operator!=(const vector4&) const;
  void print(std::ostream&) const;
  void print() const;
};

vector4 dehomogenize(vector4);

std::ostream& operator<< (std::ostream& out, const vector4& v);

} //end namespace gx
#endif