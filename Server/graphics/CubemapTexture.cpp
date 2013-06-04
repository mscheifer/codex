#include "CubemapTexture.h"

namespace {
	const GLenum cubemap_types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
									   GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
									   GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
									   GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
									   GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
									   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

	const std::string fileExt = ".jpg";
	const std::string dir = "./models/skybox_sunset/";
	const std::string right = "right" + fileExt;
	const std::string left = "left" + fileExt;
	const std::string front = "front" + fileExt;
	const std::string back = "back" + fileExt;
	const std::string top = "top" + fileExt;
	const std::string bottom = "bottom" + fileExt;
}


/**
 * tex_dir - the directory path where all skybox textures are located.
 * right_FN - right side (pos x axis) texture file name.
 * left_FN - left side (neg x axis) texture file name.
 * front_FN - front side (pos z axis) texture file name.
 * back_FN - back side (neg z axis) texture file name.
 * top_FN - top side (pos y axis) texture file name.
 * bot_FN - bottom side (neg y axis) texture file name.
 */
gx::CubemapTexture::CubemapTexture()
								   : m_fileNames(6), 
								     m_textureObj() 
{
	m_fileNames[0] = dir + left;
	m_fileNames[1] = dir + right;
	m_fileNames[2] = dir + front;
	m_fileNames[3] = dir + back;
	m_fileNames[4] = dir + top;
	m_fileNames[5] = dir + bottom;
	this->Load();
}


gx::CubemapTexture::~CubemapTexture() {}


GLuint gx::CubemapTexture::Load()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	debugout << "glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);" << endl;
	glGenTextures(1, &m_textureObj);
	debugout << "glGenTextures(1, &m_textureObj@" << &m_textureObj << ");" << endl;
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
	debugout << "glBindTexture(GL_TEXTURE_CUBE_MAP, " << m_textureObj << ");" << endl;

	sf::Image p_image;

	for (unsigned int i = 0 ; i < m_fileNames.size() ; i++) {

		// tries to load image 
		if (!p_image.loadFromFile(m_fileNames[i])) {
			std::cout << "Error loading texture '" << m_fileNames[i] << "'"<< endl;
			return 0;
		}

		sf::Vector2u dimension = p_image.getSize();
		glTexImage2D(cubemap_types[i], 0, GL_RGBA, dimension.x, dimension.y,
			0, GL_RGBA, GL_UNSIGNED_BYTE, p_image.getPixelsPtr());
		
		debugout << "glTexImage2D(" << cubemap_types[i] << ", 0, GL_RGBA, " << dimension.x 
			<< ", " << dimension.y << ",\n" << "\t" 
			<< " 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image->getPixelsPtr()" 
			<<  ");" << endl;
	} 

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	debugout << "glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);" << endl;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	debugout << "glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);" << endl;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	debugout << "glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);" << endl;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	debugout << "glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);" << endl;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	debugout << "glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);" << endl;

	return m_textureObj;
}

void gx::CubemapTexture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	debugout << "glActiveTexture(GL_TEXTURE0);" << endl;
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_textureObj);
	debugout << "glBindTexture(GL_TEXTURE_CUBE_MAP, " << m_textureObj << ");" << endl;
}
 