#ifndef DRAWERIMPL_H
#define DRAWERIMPL_H
#include <GL/glew.h>
#include <vector>
#include "bone.h"
#include "material.h"
#include "matrix.h"
#include "uniformLocation.h"
#include "vao.h"

namespace gx {
struct graphicsEntity;
class displaySet;

class staticDrawerImpl {
    uniform::mat4floc modelToWorldLoc;
  public:
    static const std::string shaderID;
    typedef graphicsEntity entity_t;
    static const std::string vertShader;
    static const std::string fragShader;
    struct entityClass {
      typedef matrix        instance;
      std::vector<instance> instances;
      std::vector<instance> staticInstances;
      vao                   vertData;
      material              mat;
      entityClass(entity_t,varSigs_t);
      entityClass(const entityClass&);// = delete;
      entityClass& operator=(const entityClass&);// = delete;
      entityClass(entityClass&&) noexcept;
      entityClass& operator=(entityClass&&);// = delete;
      void clear();
      void update(displaySet const&);
      void draw();
    };
    staticDrawerImpl(const shaderProgram&);
    void setUniforms(const entityClass&,const entityClass::instance&) const;
    void setUniforms(const entityClass::instance&) const;
    struct instanceData {
      vector4f pos;
      vector3f dirY;
      GLfloat  scale;
      unsigned int type;
    };
    static matrix makePositionMatrix(instanceData const&);
    void addInstance(const instanceData&,std::vector<entityClass>&);
    void addStaticInstance(const instanceData&,std::vector<entityClass>&);
};

class dynamicDrawerImpl {
    static const unsigned int maxBones = 30;
    staticDrawerImpl staticBase;
    uniform::mat4floc boneTransforms;
  public:
    static const std::string shaderID;
    typedef graphicsEntity entity_t;
    static const std::string vertShader;
    static const std::string fragShader;
    struct entityClass {
      struct instance {
        matrix position;
        unsigned int animation;
              double timePos;
      };
      std::vector<instance> instances;
      std::vector<instance> staticInstances;
      std::vector<double>   animationDurations;
      vao                   vertData;
      material              mat;
      bone                  rootBone;
      entityClass(entity_t,varSigs_t);
      entityClass(const entityClass&);// = delete;
      entityClass& operator=(const entityClass&);// = delete;
      entityClass(entityClass&&) noexcept;
      entityClass& operator=(entityClass&&);// = delete;
      void clear();
      void update(displaySet const&);
      void draw();
    };
    dynamicDrawerImpl(const shaderProgram&);
    void setUniforms(const entityClass&,const entityClass::instance&) const;
    struct instanceData : public staticDrawerImpl::instanceData {
      unsigned int animation;
      double timePosition;
    };
    void addInstance(instanceData const&,std::vector<entityClass>&);
    void addStaticInstance(instanceData const&,std::vector<entityClass>&);
};

} //end namespace gx
#endif // DRAWERIMPL_H
