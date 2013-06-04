#include "shaderProgram.h"
#include <array>
#include "vao.h"
#include "vertexAttrib.h"
#include "material.h"

namespace gx {
const unsigned int maxParticleSources = 5;
const unsigned int particlesPerFrame = 5;
const unsigned int particlesLifeTime = 100;
const unsigned int maxParticles = maxParticleSources * particlesPerFrame * particlesLifeTime;

struct particleEntity {
  typedef std::vector<std::shared_ptr<const vertexAttrib>> attribsList_t;
  typedef std::shared_ptr<dynamicVertexAttrib> dynamicPtr_t;
  std::vector<GLuint> indices;
  dynamicPtr_t positionAttrib;
  attribsList_t attribs; //includes the above
  material mat;
  particleEntity(material);
  particleEntity(particleEntity const&);// = delete; //don't copy
  particleEntity& operator=(particleEntity const&);// = delete; //don't copy
  particleEntity(particleEntity&&) noexcept;
  particleEntity& operator=(particleEntity&&);// = delete; //define later
};

class particleDrawerImpl {
    struct particle {
      vector4f     position;
      unsigned int lifetime;
      particle(vector4f);
    };
  public:
    static const std::string shaderID;
    typedef particleEntity entity_t;
    static const std::string vertShader;
    static const std::string fragShader;
    struct entityClass {
      typedef int instance;
      //just draw once for all of this kind of particle effect
      std::array<int,1>     instances;
      std::vector<particle> particles;
      std::shared_ptr<dynamicVertexAttrib> positionAttrib;
      unsigned int          numIndices;
      vao                   vertData;
      material     mat;
      entityClass(entity_t,varSigs_t);
      entityClass(const entityClass&);// = delete;
      entityClass& operator=(const entityClass&);// = delete;
      entityClass(entityClass&&) noexcept;
      entityClass& operator=(entityClass&&);// = delete;
      void clear();
      void update();
      void draw();
    };
    struct instanceData {
      vector4f position;
      unsigned int type;
    };
    particleDrawerImpl(const shaderProgram&);
    void setUniforms(const entityClass&,const entityClass::instance&) const;
    void addInstance(instanceData,std::vector<entityClass>&);
};
} //end namespace gx