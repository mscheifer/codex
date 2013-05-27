#ifndef VECTOR3_H
#define VECTOR3_H
#include <GL/glew.h>
#include <iostream>
#include <array>
#include "vector4.h"
#include <SFML/Network/Packet.hpp>

namespace gx {
template<typename T>
class vector4;

template<typename T>
class vector3 {
    std::array<T,3> elems;
  public:
    static constexpr unsigned int numComponents = 3;
    typedef typename std::array<T,3>::size_type index_type;
    typedef T elem_t;
    elem_t& x;
    elem_t& y;
    elem_t& z;
    vector3();
    vector3(elem_t,elem_t,elem_t);
    vector3(vector3<T> const&);
    vector3(vector3<T>&&);
    vector3<T>& operator=(vector3<T>&&);
    vector3<T>& operator=(const vector3<T>&);
    void set(elem_t, elem_t, elem_t);
    void add(const vector3<T>&);
    void add(const vector3<T>&,const vector3<T>&);
    void subtract(const vector3<T>&);
    void subtract(const vector3<T>&,const vector3<T>&);
    void negate();
    void negate(const vector3<T>&);
    void scale(elem_t s);
    void scale(elem_t s,const vector3<T>&);
    elem_t dot(const vector3<T>&) const;
    void cross(const vector3<T>&,const vector3<T>&);
    elem_t magnitudesq() const;
    elem_t magnitude() const;
    void normalize();
    std::array<elem_t,3> oglVec3() const;

    typename std::array<T,3>::iterator begin();
    typename std::array<T,3>::iterator end();
    elem_t& get(index_type);
    const elem_t& get(index_type) const;
    elem_t& operator[](index_type);
    const elem_t& operator[](index_type) const;
    vector3<T> operator-() const;
    vector3<T> operator+(const vector3<T>&) const;
    vector4<T> operator+(const vector4<T>&) const;
    vector3<T> operator-(const vector3<T>&) const;
    vector3<T> operator*(const vector3<T>&) const;
    vector3<T> operator*(elem_t) const;
    vector3<T>& operator+=(const vector3<T>&);
    void print(std::ostream&) const;
    void print() const;
    bool operator==(const vector3<T>&) const;
    bool operator!=(const vector3<T>&) const;
    void serialize(sf::Packet & packet) const;
    void deserialize(sf::Packet & packet);
};

template<typename T>
vector3<T> operator*(typename vector3<T>::elem_t, const vector3<T>&);
template<typename T>
std::ostream& operator<< (std::ostream& out, const vector3<T>& v);

typedef vector3<GLfloat> vector3f;
//typedef vector3<double> vector3d;

} //end namespace gx
#endif
