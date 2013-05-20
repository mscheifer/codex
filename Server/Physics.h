#pragma once
#include <math.h>
#include "graphics/vector3.h"
#include "graphics/vector4.h"

typedef float length_t;
typedef gx::vector3<length_t> v3_t;
typedef gx::vector4<length_t> v4_t;

const v3_t GRAVITY(0,0,-9.8f);
const v3_t ZEROVEC(0,0,0);