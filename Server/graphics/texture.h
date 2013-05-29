#ifndef TEXTURE_H
#define  TEXTURE_H
#include <GL/glew.h>
#include <string>
#include <SFML/Graphics.hpp>
#include "windowsHacks.h"

namespace gx {

struct Texture {
    Texture(GLenum TextureTarget, const std::string& FileName);
    Texture(Texture const&);
    Texture& operator=(Texture const&);
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&);
    ~Texture();

    void bind(GLenum TextureUnit) const;

private:
    bool Load();
    GLenum m_textureTarget;
    std::string m_fileName;
    sf::Image m_image;
    GLuint m_textureID;
};

} //end namespace gx
#endif  /* TEXTURE_H */

