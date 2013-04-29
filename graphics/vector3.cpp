#include "vector3.h"

gx::vector3::vector3(): x(0.0), y(0.0), z(0.0) {}

gx::vector3::vector3(elem_t x0,elem_t y0,elem_t z0): x(x0), y(y0), z(z0) {}

void gx::vector3::set(elem_t x0,elem_t y0,elem_t z0) {
  this->x = x0;
  this->y = y0;
  this->z = z0;
}

void gx::vector3::add(const vector3 &a) {
  this->x += a.x;
  this->y += a.y;
  this->z += a.z;
}

void gx::vector3::add(const vector3 &a,const vector3 &b) {
  this->x = a.x + b.x;
  this->y = a.y + b.y;
  this->z = a.z + b.z;
}

void gx::vector3::subtract(const vector3 &a) {
  this->x -= a.x;
  this->y -= a.y;
  this->z -= a.z;
}

void gx::vector3::subtract(const vector3 &a,const vector3 &b) {
  this->x = a.x - b.x;
  this->y = a.y - b.y;
  this->z = a.z - b.z;
}

void gx::vector3::negate() {
  this->x = -x;
  this->y = -y;
  this->z = -z;
}

void gx::vector3::negate(const vector3 &a) {
  this->x = -a.x;
  this->y = -a.y;
  this->z = -a.z;
}

void gx::vector3::scale(elem_t s) {
  this->x *= s;
  this->y *= s;
  this->z *= s;
}

void gx::vector3::scale(elem_t s,const vector3 &a) {
  this->x = s * a.x;
  this->y = s * a.y;
  this->z = s * a.z;
}

gx::vector3::elem_t gx::vector3::dot(const vector3 &a) const {
  return x * a.x + y * a.y + z * a.z;
}

void gx::vector3::cross(const vector3 &a,const vector3 &b) {
  this->x = a.y * b.z - a.z * b.y;
  this->y = a.z * b.x - a.x * b.z;
  this->z = a.x * b.y - a.y * b.x;
}

gx::vector3::elem_t gx::vector3::magnitude() const {
  return elem_t(sqrt(x*x + y*y + z*z));
}

void gx::vector3::normalize() {
  if(magnitude() > 0) {
    scale(elem_t(1.0)/magnitude());
  }
}

std::array<gx::vector3::elem_t,3> gx::vector3::oglVec3() const {
  std::array<gx::vector3::elem_t,3> ret = {{ this->x, this->y, this->z }};
  return ret;
}

gx::vector3::elem_t& gx::vector3::get(int i) {
  return (i == 0) ? x : (i == 1) ? y : z;
}

const gx::vector3::elem_t& gx::vector3::get(int i) const {
  return (i == 0) ? x : (i == 1) ? y : z;
}

gx::vector3::elem_t& gx::vector3::operator[](int i) {
  return get(i);
}

const gx::vector3::elem_t& gx::vector3::operator[](int i) const {
  return get(i);
}

gx::vector3 gx::vector3::operator-() const {
  vector3 r; 
  r.negate(*this);
  return r;
}

gx::vector3 gx::vector3::operator+(const vector3 &a) const {
  vector3 r;
  r.add(*this,a);
  return r;
}

gx::vector3 gx::vector3::operator-(const vector3 &a) const {
  vector3 r;
  r.subtract(*this,a);
  return r;
}

gx::vector3 gx::vector3::operator*(const vector3 &a) const {
  vector3 r;
  r.cross(*this,a);
  return r;
}

gx::vector3 gx::vector3::operator*(elem_t f) const {
  vector3 r;
  r.scale(f,*this);
  return r;
}

gx::vector3& gx::vector3::operator+=(const vector3& o) {
  this->x = this->x + o.x;
  this->y = this->y + o.y;
  this->z = this->z + o.z;
  return *this;
}

void gx::vector3::print(std::ostream& o) const {
  o << this->x << " " << this->y << " " << this->z;
}

void gx::vector3::print() const {
  this->print(std::cout);
}

bool gx::vector3::operator==(const vector3& o) const {
  return this->get(0) == o.get(0) && this->get(1) == o.get(1) &&
         this->get(2) == o.get(2);
}

bool gx::vector3::operator!=(const vector3& o) const {
  return !((*this)==o);
}

gx::vector3 operator*(gx::vector3::elem_t f, const gx::vector3& v) {
  return v * f;
}

std::ostream& operator<< (std::ostream& out, const gx::vector3& v) {
  v.print(out);
  return out;
}
