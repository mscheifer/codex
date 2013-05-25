#ifndef TEXTURE_H
#define  TEXTURE_H
#include <GL/glew.h>
#include <string>
#include <SFML/Graphics.hpp>

namespace gx {

class Texture {
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    sf::Image m_image;
};

} //end namespace gx
#endif  /* TEXTURE_H */

