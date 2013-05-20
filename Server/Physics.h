#pragma once
#include <math.h>
#include "graphics/vector3.h"
#include "graphics/vector4.h"
#include "ConfigManager.h"

typedef float length_t;
typedef gx::vector3<length_t> v3_t;
typedef gx::vector4<length_t> v4_t;

static v3_t getGravity(){ 
  //return v3_t(0,0,0);
  static v3_t GRAVITY(0,0,ConfigManager::gravity());
  return GRAVITY;
}

const v3_t ZEROVEC(0,0,0);