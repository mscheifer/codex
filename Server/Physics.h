#pragma once
#include <math.h>
#include "3DCoordinate.h"
#include "directionalVector.h"
#include "graphics\vector3.h"
#include "graphics\vector4.h"
#define SERVERCLOCK .0333333333f //TODO get this from teh server
#define GRAVITY -9.8f

typedef float length_t;
typedef gx::vector3<length_t> v3_t;

const v3_t gravity(0,0,-9.8f);

Coordinate ThreeDMovement(Coordinate location, Direction direction, Velocity accelerationZ);
Position calculateDistanceInBetween( Coordinate x, Coordinate y);
