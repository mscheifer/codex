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
  
  glTexImage2D(this->m_textureTarget, 0, GL_RGBA, width, height, 0, 
	  GL_RGBA, GL_UNSIGNED_BYTE, this->m_image.getPixelsPtr());
  glGenerateMipmap(this->m_textureTarget);
  if (type & GROUNDTEX) {

	glTexParameterf( this->m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( this->m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT );
  }
	glTexParameterf( this->m_textureTarget, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR );
  glTexParameterf(this->m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return success;
}

void gx::Texture::bind(GLenum TextureUnit) const {
  glActiveTexture(TextureUnit);
  glBindTexture(this->m_textureTarget, this->m_textureID);
}

