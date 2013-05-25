#ifndef VECTOR4_H
#define VECTOR4_H
#include <iostream>
#include <array>
#include "oglUtil.h"

namespace gx {
template<typename T>
class vector3;

template<typename T>
class vector4 {
    std::array<T,4> elems;
  public:
    typedef typename std::array<T,4>::size_type index_type;
    typedef T elem_t;
    elem_t& x;
    elem_t& y;
    elem_t& z;
    elem_t& w;
    vector4();
    vector4(vector4<T> const&);
    vector4(vector4<T>&&);
    vector4<T>& operator=(vector4<T> const&);
    vector4<T>& operator=(vector4<T>&&);
    vector4(elem_t,elem_t,elem_t);
    vector4(const std::initializer_list<elem_t>);
    void set(elem_t,elem_t,elem_t,elem_t);
    void add(const vector3<T>&);
    void add(const vector4<T>&,const vector3<T>&);
    void subtract(const vector3<T>&);
    void subtract(const vector4<T>&,const vector3<T>&);
    void dehomogenize();

    elem_t& get(index_type);
    const elem_t& get(index_type i) const;
    elem_t& operator[](index_type i); 
    const elem_t& operator[](index_type i) const;
    vector4<T> operator+(const vector3<T>&) const;
    vector3<T> operator-(vector4<T>) const;      
    vector4<T>& operator+=(const vector3<T>&);
    bool operator==(const vector4<T>&) const;
    bool operator!=(const vector4<T>&) const;
    void print(std::ostream&) const;
    void print() const;
};

template<typename T>
vector4<T> dehomogenize(vector4<T>);

template<typename T>
std::ostream& operator<< (std::ostream& out, const vector4<T>& v);

typedef vector4<GLfloat> vector4f;

} //end namespace gx
#endif
