#ifndef LOADCUSTOM_H
#define LOADCUSTOM_H
#include "graphicsEntity.h"

namespace gx {
	std::vector<graphicsEntity> loadCube();

	graphicsEntity loadSkybox();
	graphicsEntity loadGround(const float edgeLength, 
                            const float zHeight = 0.0f, 
                            std::string texPath = "models/white.png");
} //end namespace gx
#endif //LOADCUSTOM_H
