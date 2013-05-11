#ifndef TEXTURE_H
#define	TEXTURE_H
#include <string>
#include <GL/glew.h>

namespace gx {

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    //Magick::Image* m_pImage;
    //Magick::Blob m_blob;
};

} //end namespace gx
#endif	/* TEXTURE_H */

