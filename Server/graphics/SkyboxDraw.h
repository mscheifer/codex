#ifndef SKYBOXDRAW_H
#define SKYBOXDRAW_H

#include "vao.h"
#include "shaderProgram.h"
#include "CubemapTexture.h"

namespace gx {
	class SkyboxDraw {
		shaderProgram program;
		vao drawData;
		CubemapTexture texture;
		const uniform::block& VPStorage;

	public:
		SkyboxDraw(uniform::block &);

		void draw();

	};
}

#endif