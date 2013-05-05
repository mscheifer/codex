#ifndef MATRIX4_H
#define MATRIX4_H
#include <GL/glew.h>
#include <vector>
#include <array>
#include "vector4.h"
#include "vector3.h"

namespace gx {

struct matrix {
  typedef GLfloat elem_t;
private:
  //this is row major
  std::array<std::array<elem_t,4>,4> elems;
public:
  matrix();
  matrix(std::array<std::array<elem_t,4>,4> a);
  matrix(elem_t a, elem_t b, elem_t c, elem_t d, elem_t e, elem_t f, elem_t g,
    elem_t h, elem_t i, elem_t j, elem_t k, elem_t l, elem_t m, elem_t n,
    elem_t o, elem_t p);
  void print(std::ostream& o) const;
  void print() const;
  std::array<elem_t,4>& operator[](int i);
  const std::array<elem_t,4>& operator[](int i) const;
  void identify();
  void oglmatrix(elem_t a[16]) const;
  void oglmatrix(elem_t a[4][4]) const;
  std::array<elem_t,16> oglmatrix() const;
  bool operator==(const matrix& right) const;
  bool operator!=(const matrix& right) const;
};

//note mathematical ordering
matrix multiply(const matrix& left,const matrix& right);
vector4 multiply(const matrix& left,const vector4& right);
vector3 multiply(const matrix& left,const vector3& right);
matrix operator*(const matrix& left,const matrix& right);
vector4 operator*(const matrix& left,const vector4& right);
vector3 operator*(const matrix& left,const vector3& right);

matrix rotateX(double angle);
matrix rotateY(double angle);
matrix rotateZ(double angle);
matrix rotateArbitrary(vector3 axis, double angle);
matrix scaling(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z);
matrix scalingMatrix(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z);
matrix translation(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z);
//converts a vector from the standard basis to the supplied basis
//params: x y and z vectors
matrix toBasis(vector3,vector3,vector3);

const matrix identity = matrix(1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1);

std::ostream& operator<< (std::ostream& out, const matrix&);
} //end namespace gx
#endif
