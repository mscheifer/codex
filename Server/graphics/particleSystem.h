#include "shaderProgram.h"

namespace gx {
class particleSystem {
    attribsList_t attribs; //store references to these here and pass them
                           //to the drawer
    drawer<particleDrawerImpl> drawer;
  public:
    void update();
    void draw();
};

class particleDrawerImpl {
  public:
    static const std::string shaderID;
    typedef particleEntity entity_t;
    static const std::string vertShader;
    static const std::string fragShader;
    struct entityClass {
      struct instance {
        matrix position
        attribsList_t attribs;
      };
      std::vector<instance> instances;
      vao                   vertData;
      material              mat;
      entityClass(entity_t,varSigs_t);
      entityClass(const entityClass&);// = delete;
      entityClass& operator=(const entityClass&);// = delete;
      entityClass(entityClass&&) noexcept;
      entityClass& operator=(entityClass&&);// = delete;
      void clear();
    };
    struct instanceData {
      vector3f pos;
      unsigned int type; //projectile type
    };
    particleSystem(const shaderProgram&);
    void addInstance(instanceData,std::vector<entityClass>&);
};
} //end namespace gx