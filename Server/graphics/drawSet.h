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
    uniform::mat4floc        modelToWorldLoc;
  public:
    static const std::string vertShader;
    static const std::string fragShader;
    struct entityClass {
      typedef matrix        instance;
      std::vector<instance> instances;
      vao                   vertData;
      entityClass(staticEntity,std::map<std::string,vertexAttribSignature>);
      entityClass(const entityClass&) = delete;
      entityClass(entityClass&&);
      entityClass& operator=(const entityClass&) = delete;
      entityClass& operator=(entityClass&&) = delete;
      void clear();
    };
    void setUniforms(const entityClass::instance&) const;
    struct instanceData {
      vector3f pos;
      vector3f dirY;
      unsigned int type;
    };
    staticDrawerImpl(const shaderProgram&);
    void addInstance(instanceData,std::vector<entityClass>&);
};

} //end namespace gx
#endif // DRAWSET_H
