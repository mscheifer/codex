#pragma once
#include <math.h>
#include "3DCoordinate.h"
#include "directionalVector.h"
#define SERVERCLOCK .0333333333
#define GRAVITY -9.8
static Coordinate ThreeDMovement(Coordinate location, Direction direction, float accelerationZ);