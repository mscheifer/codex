#ifndef DRAWSET_H
#define DRAWSET_H
#include <GL/glew.h>
#include <vector>
#include "vao.h"
#include "matrix.h"
#include "uniformLocation.h"
#include "graphicsEntity.h"

namespace gx {
class staticDrawerImpl {
    uniform::mat4floc modelToWorldLoc;
  public:
    typedef staticEntity entity_t;
    static const std::string vertShader;
    static const std::string fragShader;
    struct entityClass {
      typedef matrix        instance;
      std::vector<instance> instances;
      vao                   vertData;
      matrix                centerAndResize;
      entityClass(entity_t,varSigs_t);
      entityClass(const entityClass&);// = delete;
      entityClass& operator=(const entityClass&);// = delete;
      entityClass(entityClass&&) noexcept;
      entityClass& operator=(entityClass&&);// = delete;
      void clear();
    };
    staticDrawerImpl(const shaderProgram&);
    void setUniforms(const entityClass&,const entityClass::instance&) const;
    void setUniforms(const entityClass::instance&) const;
    struct instanceData {
      vector3f pos;
      vector3f dirY;
      unsigned int type;
    };
    void addInstance(instanceData,std::vector<entityClass>&);
};

class dynamicDrawerImpl {
    static const unsigned int maxBones = 30;
    staticDrawerImpl staticBase;
    uniform::mat4floc boneTransforms;
  public:
    typedef dynamicEntity entity_t;
    static const std::string vertShader;
    static const std::string fragShader;
    struct entityClass {
      struct instance {
        matrix position;
        unsigned int animation;
        unsigned int timePos;
      };
      std::vector<instance> instances;
      vao                   vertData;
      bone                  rootBone;
      entityClass(entity_t,varSigs_t);
      entityClass(const entityClass&);// = delete;
      entityClass& operator=(const entityClass&);// = delete;
      entityClass(entityClass&&) noexcept;
      entityClass& operator=(entityClass&&);// = delete;
      void clear();
    };
    dynamicDrawerImpl(const shaderProgram&);
    void setUniforms(const entityClass&,const entityClass::instance&) const;
    struct instanceData : public staticDrawerImpl::instanceData {
      unsigned int animation;
      unsigned int timePosition;
    };
    void addInstance(instanceData,std::vector<entityClass>&);
};

} //end namespace gx
#endif // DRAWSET_H
