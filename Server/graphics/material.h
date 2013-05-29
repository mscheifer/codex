#ifndef MATERIAL_H
#define MATERIAL_H
#include "texture.h"
#include "vector4.h"

namespace gx {
class shaderProgram;

class material {
    Texture  diffuseTex;
    vector4f diffuseColor;
  public:
    material(Texture,vector4f);
    void bind() const;

    static void setupBindings(shaderProgram const&);
};
} //end namespace gx
#endif //MATERIAL_H
