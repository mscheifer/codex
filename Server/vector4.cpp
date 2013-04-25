#include "vector4.h"

gx::vector4::vector4(): x(0.0), y(0.0), z(0.0), w(1) {}

gx::vector4::vector4(elem_t x0,elem_t y0,elem_t z0): x(x0), y(y0), z(z0), w(1){}
/*
gx::vector4::vector4(const std::initializer_list<elem_t> l)
           : x(l.begin()   != l.end() ? *(l.begin())   : 0),
             y(l.begin()+1 != l.end() ? *(l.begin()+1) : 0),
             z(l.begin()+2 != l.end() ? *(l.begin()+2) : 0),
             w(1) {}
             */
void gx::vector4::set(elem_t x0,elem_t y0,elem_t z0,elem_t w0) {
  this->x = x0;
  this->y = y0;
  this->z = z0;
  this->w = w0;
}

void gx::vector4::add(const vector3 &a) {
  this->x += a.x;
  this->y += a.y;
  this->z += a.z;
}

void gx::vector4::add(const vector4 &a,const vector3 &b) {
  this->x = a.x + b.x;
  this->y = a.y + b.y;
  this->z = a.z + b.z;
  this->w = 1;
}

void gx::vector4::subtract(vector4 &a) {
  this->x -= a.x;
  this->y -= a.y;
  this->z -= a.z;
  this->w -= a.w;
}

void gx::vector4::subtract(vector4 &a,vector4 &b) {
  this->x = a.x - b.x;
  this->y = a.y - b.y;
  this->z = a.z - b.z;
  this->w = a.w - b.w;
}

void gx::vector4::dehomogenize() {
  this->x = x / w;
  this->y = y / w;
  this->z = z / w;
  this->w = 1;
}

gx::vector4::elem_t& gx::vector4::get(int i) {
  return vector4::index<elem_t>(*this,i);
}  

const gx::vector4::elem_t& gx::vector4::get(int i) const {
  return vector4::index<const elem_t>(*this,i);
}

gx::vector4::elem_t& gx::vector4::operator[](int i) {
  return this->get(i);
}

const gx::vector4::elem_t& gx::vector4::operator[](int i) const {
  return this->get(i);
}

gx::vector4 gx::dehomogenize(gx::vector4 a) {
  a.dehomogenize();
  return a;
}

gx::vector4 gx::vector4::operator+(const vector3& a) const {
  vector4 r;
  r.add(*this,a);
  return r;
}

gx::vector3 gx::vector4::operator-(vector4 a) const {
  vector4 r = *this;
  r.dehomogenize();
  a.dehomogenize();
  r.subtract(a);
  return vector3(r.x,r.y,r.z);
}

bool gx::vector4::operator==(const vector4& a) const {
  //think about a better way to do this with floating point types
  return this->x == a.x && this->y == a.y && this->z == a.z && this->w == a.w;
}

bool gx::vector4::operator!=(const vector4& a) const {
  return !(*this == a);
}

void gx::vector4::print(std::ostream& o) const {
  o << this->x << " " << this->y << " " << this->z  << " " << this->w;
}

void gx::vector4::print() const {
  this->print(std::cout);
}

std::ostream& gx::operator<< (std::ostream& out, const vector4& v) {
  v.print(out);
  return out;
}