#ifndef GRAPHICSENTITY_H
#define GRAPHICSENTITY_H
#include <vector>
#include "vertexAttrib.h"
#include "bone.h"

namespace gx {
struct staticEntity {
  typedef vertexAttrib::attribsList_t attribsList_t;
  std::vector<GLuint> indices;
        attribsList_t attribs;

  staticEntity(std::vector<GLuint>, attribsList_t);
  staticEntity(const staticEntity&);// = delete; //don't copy
  staticEntity& operator=(const staticEntity&);// = delete; //don't copy
  staticEntity(staticEntity&&) noexcept;
  staticEntity& operator=(staticEntity&&);
};

struct dynamicEntity : public staticEntity {
  //bone tree
  bone rootBone;
  dynamicEntity(std::vector<GLuint>, attribsList_t, bone);
  dynamicEntity(const dynamicEntity&);// = delete; //don't copy
  dynamicEntity& operator=(const dynamicEntity&);// = delete; //don't assign
  dynamicEntity(dynamicEntity&&) noexcept;
  dynamicEntity& operator=(dynamicEntity&&);
  dynamicEntity(staticEntity&&);
  dynamicEntity& operator=(staticEntity&&);
};

} //end namespace gx
#endif //GRAPHICSENTITY_H
