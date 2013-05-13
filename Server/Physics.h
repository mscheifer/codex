#pragma once
#include <math.h>
#include "graphics\vector3.h"
#include "graphics\vector4.h"

typedef float length_t;
typedef gx::vector3<length_t> v3_t;

const v3_t gravity(0,0,-9.8f);
const v3_t zeroVec(0,0,0);

//Coordinate ThreeDMovement(Coordinate location, Direction direction, Velocity accelerationZ);
//Position calculateDistanceInBetween( Coordinate x, Coordinate y);
