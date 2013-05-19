#pragma once
#include <math.h>
#include "graphics/vector3.h"
#include "graphics/vector4.h"

typedef float length_t;
typedef gx::vector3<length_t> v3_t;

const v3_t GRAVITY(0,0,-9.8*2.5f);
const v3_t ZEROVEC(0,0,0);

//Coordinate ThreeDMovement(Coordinate location, Direction direction, Velocity accelerationZ);
//Position calculateDistanceInBetween( Coordinate x, Coordinate y);
