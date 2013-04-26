#pragma once
#include <math.h>
#include "3DCoordinate.h"
#include "directionalVector.h"
#define SERVERCLOCK .0333333333f
#define GRAVITY -9.8f
Coordinate ThreeDMovement(Coordinate location, Direction direction, Velocity accelerationZ);
Position calculateDistanceInBetween( Coordinate x, Coordinate y);
