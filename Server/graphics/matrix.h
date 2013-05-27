#ifndef MATRIX4_H
#define MATRIX4_H
#include <GL/glew.h>
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
  typedef typename std::array<std::array<elem_t,4>,4>::size_type row_index_type;
  typedef typename            std::array<elem_t,4>::size_type column_index_type;
  matrix();
  matrix(std::array<std::array<elem_t,4>,4> a);
  matrix(elem_t a, elem_t b, elem_t c, elem_t d, elem_t e, elem_t f, elem_t g,
    elem_t h, elem_t i, elem_t j, elem_t k, elem_t l, elem_t m, elem_t n,
    elem_t o, elem_t p);
  void print(std::ostream& o) const;
  void print() const;
  std::array<elem_t,4>& operator[](row_index_type i);
  const std::array<elem_t,4>& operator[](row_index_type i) const;
  void identify();
  void oglmatrix(elem_t a[16]) const;
  void oglmatrix(elem_t a[4][4]) const;
  std::array<elem_t,16> oglmatrix() const;
  bool operator==(const matrix& right) const;
  bool operator!=(const matrix& right) const;
  matrix& operator+=(const matrix& right);
};

//note mathematical ordering
matrix multiply(const matrix& left,const matrix& right);
matrix multiply(matrix left,matrix::elem_t right);
vector4f multiply(const matrix& left,const vector4f& right);
vector3f multiply(const matrix& left,const vector3f& right);
matrix operator*(const matrix& left,const matrix& right);
matrix operator*(matrix left,matrix::elem_t right);
vector4f operator*(const matrix& left,const vector4f& right);
vector3f operator*(const matrix& left,const vector3f& right);

matrix rotateX(double angle);
matrix rotateY(double angle);
matrix rotateZ(double angle);
matrix rotateArbitrary(vector3f axis, double angle);
matrix scaling(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z);
matrix scalingMatrix(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z);
matrix uniformScaling(matrix::elem_t x);
matrix translation(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z);
//converts a vector from the standard basis to the supplied basis
//params: x y and z vectors
matrix toBasis(vector3f const&,vector3f const&,vector3f const&);
matrix toRightHandBasisFromYandUp(vector3f const&,vector3f const&);

const matrix identity = matrix(1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1);

std::ostream& operator<< (std::ostream& out, const matrix&);
} //end namespace gx
#endif
