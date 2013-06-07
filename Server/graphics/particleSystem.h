#include "shaderProgram.h"
#include <array>
#include "vao.h"
#include "vertexAttrib.h"
#include "material.h"
#include "vector3.h"

namespace gx {
const unsigned int maxParticleSources = 10;
const unsigned int particlesPerFrame = 1;
const unsigned int particlesLifeTime = 750;
const unsigned int maxParticles = maxParticleSources * particlesPerFrame * particlesLifeTime;

struct particleEntity {
  typedef std::vector<std::shared_ptr<const vertexAttrib>> attribsList_t;
  typedef std::shared_ptr<dynamicVertexAttrib> dynamicPtr_t;
  std::vector<GLuint> indices;
  dynamicPtr_t positionAttrib;
  dynamicPtr_t colorAttrib;
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
      vector3f     velocity;
      unsigned int lifetime;
      particle(vector4f, vector3f);
    };
  public:
    static const std::string shaderID;
    typedef particleEntity entity_t;
    static const std::string vertShader;
    static const std::string fragShader;
    class entityClass {
        void addParticle(vector4f const&);
      public:
        typedef vector4f instance;
        //just draw once for all of this kind of particle effect
        std::array<vector4f,1> instances;
        std::array<vector4f,0> staticInstances;
        std::vector<vector4f>  realInstances;
        std::vector<vector4f>  realStaticInstances;
        std::vector<particle>  particles;
        std::shared_ptr<dynamicVertexAttrib> positionAttrib;
        std::shared_ptr<dynamicVertexAttrib> colorAttrib;
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
    void addInstance(instanceData const&,std::vector<entityClass>&);
    void addStaticInstance(instanceData const&,std::vector<entityClass>&);
};
} //end namespace gx