#ifndef TEXTURE_H
#define  TEXTURE_H
#include <GL/glew.h>
#include <string>
#include <SFML/Graphics.hpp>

namespace gx {

struct Texture {
    Texture(GLenum TextureTarget, const std::string& FileName);
    ~Texture();

    bool Load();

    void bind(GLenum TextureUnit) const;

private:
    GLenum m_textureTarget;
    std::string m_fileName;
    sf::Image m_image;
    GLuint m_textureObj;
    GLuint m_textureID;
};

} //end namespace gx
#endif  /* TEXTURE_H */

