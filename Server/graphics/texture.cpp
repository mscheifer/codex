#include <iostream>
#include "texture.h"
#include "oglUtil.h"

gx::Texture::Texture(GLenum TextureTarget, const std::string& FileName)
  : m_textureTarget(TextureTarget), m_fileName(FileName), m_image(), m_textureID(0) {}

gx::Texture::~Texture() {
  /*
  glDeleteTextures(1, &(this->m_textureID));
  debugout << "glDeleteTextures(1, &(this->m_textureID): " << this->m_textureID;
  debugout << ");" << endl;
  */
}

bool gx::Texture::Load() { //TODO: just move this function to the constructor
  std::cout << "loading tex" << std::endl;
  bool success = this->m_image.loadFromFile(this->m_fileName);

  if (!success) {
 		std::cout << "Error loading texture '" << this->m_fileName << "'" << std::endl;
    this->m_image.loadFromFile("white.png");
  } else {
    std::cout << "Loaded texture '" << this->m_fileName << "'" << std::endl; 
  }

  sf::Vector2u dimension = this->m_image.getSize();
  GLsizei width = dimension.x;
  GLsizei height = dimension.y;

  glGenTextures(1, &(this->m_textureID));
  glBindTexture(this->m_textureTarget, this->m_textureID);
  glTexImage2D(this->m_textureTarget, 0, GL_RGBA, width, height, 0, 
    GL_RGBA, GL_UNSIGNED_BYTE, this->m_image.getPixelsPtr());
  glTexParameterf(this->m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //TODO: change to use mipmaps
  glTexParameterf(this->m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return success;
}

void gx::Texture::bind(GLenum TextureUnit) const {
  std::cout << "binding" << std::endl;
  glActiveTexture(TextureUnit);
  glBindTexture(this->m_textureTarget, this->m_textureID);
}
