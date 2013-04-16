#include <iostream>
#include <cmath>
#include "matrix.h"

gx::matrix::matrix(): elems{{ {{ 0, 0, 0, 0 }},
                              {{ 0, 0, 0, 0 }},
                              {{ 0, 0, 0, 0 }},
                              {{ 0, 0, 0, 0 }} }} {}

gx::matrix::matrix(std::array<std::array<elem_t,4>,4> a): elems(std::move(a)) {}

gx::matrix::matrix(elem_t a, elem_t b, elem_t c, elem_t d, elem_t e, elem_t f,
         elem_t g, elem_t h, elem_t i, elem_t j, elem_t k, elem_t l, elem_t m,
         elem_t n, elem_t o, elem_t p): elems{{ {{ a, b, c, d }},
                                                {{ e, f, g, h }},
                                                {{ i, j, k, l }},
                                                {{ m, n, o, p }} }} {}

void gx::matrix::print(std::ostream& o) const {
  for(const auto& row : elems) {
    for(const auto& e : row) {
      o << e << " ";
    }
    o << std::endl;
  }
}

void gx::matrix::print() const {
  this->print(std::cout);
}

std::array<gx::matrix::elem_t,4>& gx::matrix::operator[](int i) {
  return elems[i];
}

const std::array<gx::matrix::elem_t,4>& gx::matrix::operator[](int i) const {
  return elems[i];
}

void gx::matrix::identify() {
  elems = {{ {{ 1, 0, 0, 0 }},
             {{ 0, 1, 0, 0 }},
             {{ 0, 0, 1, 0 }},
             {{ 0, 0, 0, 1 }} }};
}

void gx::matrix::oglmatrix(elem_t a[16]) const {
  //array is passed by reference by default
  for(int i = 0; i < 4 ; i++) {
    for(int j = 0; j < 4 ; j++) {
      a[i+4*j] = elems[i][j];
    }
  }
}

void gx::matrix::oglmatrix(elem_t a[4][4]) const {
  for(int i = 0; i < 4 ; i++) {
    for(int j = 0; j < 4 ; j++) {
      a[j][i] = elems[i][j];
    }
  }
}

std::array<gx::matrix::elem_t,16> gx::matrix::oglmatrix() const {
  std::array<elem_t,16> ret;
  for(int i = 0; i < 4 ; i++) {
    for(int j = 0; j < 4 ; j++) {
      ret[j*4+i] = elems[i][j];
    }
  }
  return ret;
}

bool gx::matrix::operator==(const matrix& right) const {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if((*this)[i][j] != right[i][j]) return false;
    }
  }
  return true;
}

bool gx::matrix::operator!=(const matrix& right) const {
  return !((*this) == right);
}

gx::matrix gx::multiply(const matrix& left,const matrix& right) {
  matrix result = matrix(); //0 init
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      for(int k = 0; k < 4; k++) {
        result[i][j] += left[i][k] * right[k][j];
      }
    }
  }
  return result;
}

gx::vector4 gx::multiply(const matrix& left,const vector4& right) {
  vector4 result = vector4(); //0 init
  result.w = 0;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      result[i] += left[i][j] * right[j];
    }
  }
  return result;
}

gx::vector3 gx::multiply(const matrix& left,const vector3& right) {
  vector4 temp(right[0],right[1],right[2]);
  temp[3] = 0;
  temp = left * temp;
  return vector3(temp[0],temp[1],temp[2]);
}

gx::matrix gx::operator*(const matrix& left,const matrix& right) {
  return multiply(left,right);
}

gx::vector4 gx::operator*(const matrix& left,const vector4& right) {
  return multiply(left,right);
}

gx::vector3 gx::operator*(const matrix& left,const vector3& right) {
  return multiply(left,right);
}

gx::matrix gx::rotateX(double angle) {
  return matrix(
                1,0,0,0,
                0,cos(angle),-sin(angle),0,
                0,sin(angle),cos(angle),0, 
                0,0,0,1);
}

gx::matrix gx::rotateY(double angle) {
  return matrix(
                cos(angle),0,sin(angle),0,
                0,1,0,0,
                -sin(angle),0,cos(angle),0, 
                0,0,0,1);
}

gx::matrix gx::rotateZ(double angle) {
  return matrix(
                cos(angle),-sin(angle),0,0,
                sin(angle),cos(angle),0,0, 
                0,0,1,0,
                0,0,0,1);
}

gx::matrix gx::rotateArbitrary(vector3 axis, double angle) {
  axis.normalize();
  return matrix(
                1 + (1-cos(angle))*(axis[0]*axis[0] - 1),
                -axis[2] * sin(angle) + (1 - cos(angle)) * axis[0] * axis[1],
                axis[1] * sin(angle) + (1 - cos(angle)) * axis[0] * axis[2],
                0,

                axis[2] * sin(angle) + (1 - cos(angle)) * axis[1] * axis[0],
                1 + (1 - cos(angle)) * (axis[1] * axis[1] - 1),
                -axis[0] * sin(angle) + (1 - cos(angle)) * axis[1] * axis[2],
                0, 

                -axis[1] * sin(angle) + (1 - cos(angle)) * axis[2] * axis[0],
                axis[0] * sin(angle) + (1 - cos(angle)) * axis[2] * axis[1],
                1 + (1 - cos(angle)) * (axis[2] * axis[2] - 1),
                0,

                0,0,0,1);
}

gx::matrix gx::scaling(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z) {
  return matrix(
                x,0,0,0,
                0,y,0,0,
                0,0,z,0,
                0,0,0,1);
}

gx::matrix gx::scalingMatrix(matrix::elem_t x, matrix::elem_t y,
                                               matrix::elem_t z) {
  return scaling(x, y, z);
}

gx::matrix gx::translation(matrix::elem_t x, matrix::elem_t y,
                                             matrix::elem_t z) {
  return matrix(
                1,0,0,x,
                0,1,0,y,
                0,0,1,z,
                0,0,0,1);
}

std::ostream& gx::operator<< (std::ostream& out, const matrix& m) {
  m.print(out);
  return out;
}
