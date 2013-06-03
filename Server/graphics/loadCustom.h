#ifndef LOADCUSTOM_H
#define LOADCUSTOM_H
#include "graphicsEntity.h"

namespace gx {
	std::vector<graphicsEntity> loadCube();

	graphicsEntity loadSkybox();
	graphicsEntity loadGround(const float zLevel, 
							  std::string texPath = "models/white.png",
							  unsigned int mipmapLevel = 0);
} //end namespace gx
#endif //LOADCUSTOM_H
