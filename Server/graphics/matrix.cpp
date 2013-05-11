#include <iostream>
#include <cmath>
#include "matrix.h"

gx::matrix::matrix(): elems() {
	/*elems({{ {{ 1, 0, 0, 0 }},
               {{ 0, 1, 0, 0 }},
               {{ 0, 0, 1, 0 }},
               {{ 0, 0, 0, 1 }} }})*/
	for(size_t i = 0; i < elems.size(); i++) {
		for(size_t j = 0; j < elems[i].size(); j++) {
			elems[i][j] = 0;
		}
	}
}

gx::matrix::matrix(std::array<std::array<elem_t,4>,4> a): elems(std::move(a)) {}

gx::matrix::matrix(elem_t a, elem_t b, elem_t c, elem_t d, elem_t e, elem_t f,
         elem_t g, elem_t h, elem_t i, elem_t j, elem_t k, elem_t l, elem_t m,
         elem_t n, elem_t o, elem_t p) 
                                     /*: elems{{ {{ a, b, c, d }},
                                                {{ e, f, g, h }},
                                                {{ i, j, k, l }},
                                                {{ m, n, o, p }} }}*/ {
  //visual c++ crap
  elems[0][0] = a;
  elems[0][1] = b;
  elems[0][2] = c;
  elems[0][3] = d;
  elems[1][0] = e;
  elems[1][1] = f;
  elems[1][2] = g;
  elems[1][3] = h;
  elems[2][0] = i;
  elems[2][1] = j;
  elems[2][2] = k;
  elems[2][3] = l;
  elems[3][0] = m;
  elems[3][1] = n;
  elems[3][2] = o;
  elems[3][3] = p;
}

void gx::matrix::print(std::ostream& o) const {
  //cant use range based for here because of visual c++
  for(auto rowp = elems.begin(); rowp != elems.end(); ++rowp) {
    const auto& row = *rowp;
	//cant use range based for here because of visual c++
    for(auto ep = row.begin(); ep != row.end(); ++ep) {
      const auto& e = *ep;
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
  /*elems = {{ {{ 1, 0, 0, 0 }},
               {{ 0, 1, 0, 0 }},
               {{ 0, 0, 1, 0 }},
               {{ 0, 0, 0, 1 }} }};*/
  //have to use this crap becuase of visual c++
	for(size_t i = 0; i < elems.size(); i++) {
		for(size_t j = 0; j < elems[i].size(); j++) {
			elems[i][j] = 0;
		}
	}
	for(size_t i = 0; i < elems.size(); i++) {
		elems[i][i] = 1;
	}
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

gx::vector4f gx::multiply(const matrix& left,const vector4f& right) {
  vector4f result = vector4f(); //0 init
  result.w = 0;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      result[i] += left[i][j] * right[j];
    }
  }
  return result;
}

gx::vector3f gx::multiply(const matrix& left,const vector3f& right) {
  vector4f temp(right[0],right[1],right[2]);
  temp[3] = 0;
  temp = left * temp;
  return vector3f(temp[0],temp[1],temp[2]);
}

gx::matrix gx::operator*(const matrix& left,const matrix& right) {
  return multiply(left,right);
}

gx::vector4f gx::operator*(const matrix& left,const vector4f& right) {
  return multiply(left,right);
}

gx::vector3f gx::operator*(const matrix& left,const vector3f& right) {
  return multiply(left,right);
}

gx::matrix gx::rotateX(double angle) {
  typedef gx::matrix::elem_t elem_t;
  return matrix(1,                 0,                  0,0,
                0,elem_t(cos(angle)),elem_t(-sin(angle)),0,
                0,elem_t(sin(angle)),elem_t( cos(angle)),0, 
                0,                 0,                  0,1);
}

gx::matrix gx::rotateY(double angle) {
  typedef gx::matrix::elem_t elem_t;
  return matrix(elem_t( cos(angle)),0,elem_t(sin(angle)),0,
                                  0,1,                 0,0,
                elem_t(-sin(angle)),0,elem_t(cos(angle)),0, 
                                  0,0,                 0,1);
}

gx::matrix gx::rotateZ(double angle) {
  typedef gx::matrix::elem_t elem_t;
  return matrix(elem_t(cos(angle)),elem_t(-sin(angle)),0,0,
                elem_t(sin(angle)),elem_t( cos(angle)),0,0, 
                0,               0,                    1,0,
                0,               0,                    0,1);
}

gx::matrix gx::rotateArbitrary(vector3f axis, double angle) {
  typedef gx::matrix::elem_t elem_t;
  axis.normalize();
  return matrix(
   1 + (1 - elem_t(cos(angle))) * (axis[0] * axis[0] - 1),
   -axis[2] * elem_t(sin(angle)) + (1 - elem_t(cos(angle))) * axis[0] * axis[1],
    axis[1] * elem_t(sin(angle)) + (1 - elem_t(cos(angle))) * axis[0] * axis[2],
   0,

    axis[2] * elem_t(sin(angle)) + (1 - elem_t(cos(angle))) * axis[1] * axis[0],
   1 + (1 - elem_t(cos(angle))) * (axis[1] * axis[1] - 1),
   -axis[0] * elem_t(sin(angle)) + (1 - elem_t(cos(angle))) * axis[1] * axis[2],
   0, 

   -axis[1] * elem_t(sin(angle)) + (1 - elem_t(cos(angle))) * axis[2] * axis[0],
    axis[0] * elem_t(sin(angle)) + (1 - elem_t(cos(angle))) * axis[2] * axis[1],
   1 + (1 - elem_t(cos(angle))) * (axis[2] * axis[2] - 1),
   0,

   0,0,0,1);
}

gx::matrix gx::scaling(matrix::elem_t x, matrix::elem_t y, matrix::elem_t z) {
  return matrix(x,0,0,0,
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
  return matrix(1,0,0,x,
                0,1,0,y,
                0,0,1,z,
                0,0,0,1);
}

gx::matrix gx::toBasis(const vector3f& x,const vector3f& y,const vector3f& z) {
  return matrix(x.x, y.x, z.x, 0,
                x.y, y.y, z.y, 0,
                x.z, y.z, z.z, 0,
                0,   0,   0,   1);
}

gx::matrix gx::toRightHandBasisFromY(const vector3f& dirY) {
  vector3f dirX( dirY.y,-dirY.x,      0);
  if(dirX.magnitude() > 0) {
    dirX.scale(dirY.magnitude() / dirX.magnitude());
  } else {
    dirX = vector3f(1,0,0);
  }
  vector3f dirZ(      0,-dirY.z, dirY.y);
  if(dirZ.magnitude() > 0) {
    dirZ.scale(dirY.magnitude() / dirZ.magnitude());
  } else {
    dirZ = vector3f(0,0,1);
  }
  return toBasis(dirX,dirY,dirZ);
}

std::ostream& gx::operator<< (std::ostream& out, const matrix& m) {
  m.print(out);
  return out;
}
