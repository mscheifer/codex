#ifndef CUBEMAPTEXTURE_H
#define CUBEMAPTEXTURE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "oglUtil.h"

namespace gx {
	class CubemapTexture
	{
	public:
		CubemapTexture();

		~CubemapTexture();

		void Bind(GLenum TextureUnit);

	private:
		// Load() returns the cubemap's texture id for binding later,
		// or returns 0 (invalid texture id) if loading failed at any point
		GLuint Load();
		
		std::vector<std::string> m_fileNames;
		GLuint m_textureObj;
	};

} //end namespace gx
#endif
