#include <iostream>
#include "texture.h"
#include "oglUtil.h"

gx::Texture::Texture(GLenum TextureTarget, const std::string& FileName,
					 Texture_Type matType)
  : m_textureTarget(TextureTarget), m_fileName(FileName), m_image(),
    m_textureID(0) {
	this->Load(matType);
}

gx::Texture::Texture(Texture&& other) noexcept
  : m_textureTarget(std::move(other.m_textureTarget)),
    m_fileName(std::move(other.m_fileName)), m_image(std::move(other.m_image)),
    m_textureID(std::move(other.m_textureID)) {
  other.m_textureID = 0; //delete textures won't complain
}

gx::Texture::~Texture() {
  glDeleteTextures(1, &(this->m_textureID));
  debugout << "glDeleteTextures(1, &(this->m_textureID): " << this->m_textureID;
  debugout << ");" << endl;
}

bool gx::Texture::Load(Texture_Type type) { //TODO: just move this function to the constructor
  bool success = this->m_image.loadFromFile(this->m_fileName);

  if (!success) {
 		std::cout << "Error loading texture \"" << this->m_fileName << "\"";
    std::cout << std::endl;
    this->m_image.loadFromFile("models/white.png");
  } else {
    std::cout << "Loaded texture '" << this->m_fileName << "'" << std::endl; 
  }

  sf::Vector2u dimension = this->m_image.getSize();
  GLsizei width  = static_cast<GLsizei>(dimension.x);
  GLsizei height = static_cast<GLsizei>(dimension.y);

  glGenTextures(1, &(this->m_textureID));
  glBindTexture(this->m_textureTarget, this->m_textureID);
  
  if (type & GROUNDTEX) {

	  unsigned int mmLevel = type ^ GROUNDTEX;
	  //LoadMipmap(mmLevel);
	  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 

	  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		  GL_LINEAR_MIPMAP_LINEAR );
	  glGenerateMipmap(GL_TEXTURE_2D);

	  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	  glTexImage2D(this->m_textureTarget, 0, GL_RGBA, width, height, 0, 
		  GL_RGBA, GL_UNSIGNED_BYTE, this->m_image.getPixelsPtr());
  } else {
	  glTexParameterf(this->m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameterf(this->m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  
	  glTexImage2D(this->m_textureTarget, 0, GL_RGBA, width, height, 0, 
		  GL_RGBA, GL_UNSIGNED_BYTE, this->m_image.getPixelsPtr());
  }

  return success;
}

void gx::Texture::bind(GLenum TextureUnit) const {
  glActiveTexture(TextureUnit);
  glBindTexture(this->m_textureTarget, this->m_textureID);
}

void gx::Texture::LoadMipmap(unsigned int maxLevels) {
	int substr_i = m_fileName.find_last_of(".");
	std::string imagePath = m_fileName.substr(0, substr_i);
	std::string extension = m_fileName.substr(substr_i);
	sf::Image loader;

	//set texture environment parameters
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 
		GL_MODULATE ); 

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	for (unsigned int i = 0; i <= maxLevels; ++i) {

		// the mipmap textures should be named in the convention of 
		// path + mipmap_level + image_extension
		char mipmapPath_c[50];
		sprintf(mipmapPath_c, "%s%d%s", imagePath.c_str(), i, extension.c_str());

		// load the mipmap texture image
		bool success = loader.loadFromFile(mipmapPath_c);
		if (!success) {
			std::cout << "can't load mipmap image: \"" << mipmapPath_c << "\"" << std::endl;
			continue;
		} else {
			std::cout << "loaded mipmap image: \"" << mipmapPath_c << "\"\n";
		}

		sf::Vector2u dimension = this->m_image.getSize();
		GLsizei width  = static_cast<GLsizei>(dimension.x);
		GLsizei height = static_cast<GLsizei>(dimension.y);

		// adds the mipmap to the current level of current texture
		glTexImage2D(this->m_textureTarget, i, GL_RGBA, width, height, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, loader.getPixelsPtr());
	}
}
