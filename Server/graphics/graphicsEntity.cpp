#include "graphicsEntity.h"

gx::staticEntity::staticEntity(std::vector<GLuint> ind, attribsList_t attr, 
  matrix resize): indices(std::move(ind)), attribs(std::move(attr)), 
  centerAndResize(std::move(resize)) {}

gx::staticEntity::staticEntity(staticEntity&& other) noexcept
  : indices(std::move(other.indices)), attribs(std::move(other.attribs)),
    centerAndResize(std::move(other.centerAndResize)) {}

gx::staticEntity& gx::staticEntity::operator=(staticEntity&& other) {
          this->indices = std::move(other.indices);
          this->attribs = std::move(other.attribs);
  this->centerAndResize = std::move(other.centerAndResize);
  return *this;
}

gx::dynamicEntity::dynamicEntity(std::vector<GLuint> is, attribsList_t attrs,
  bone bones): staticEntity(std::move(is),std::move(attrs),bones.transform), 
  rootBone(std::move(bones)) {}

gx::dynamicEntity::dynamicEntity(dynamicEntity&& other) noexcept
  : staticEntity(std::move(other)), rootBone(std::move(other.rootBone)) {}

gx::dynamicEntity& gx::dynamicEntity::operator=(dynamicEntity&& other) {
  this->rootBone = std::move(other.rootBone);
  staticEntity::operator=(std::move(other));
  return *this;
}

gx::dynamicEntity::dynamicEntity(staticEntity&& other)
  : staticEntity(std::move(other)),
    rootBone(-1,identity,centerAndResize,false,
      std::vector<std::vector<bone::key>>(),std::vector<bone>()) {}

gx::dynamicEntity& gx::dynamicEntity::operator=(staticEntity&& other) {
  staticEntity::operator=(std::move(other));
  rootBone = bone(-1,identity,centerAndResize,false,
    std::vector<std::vector<bone::key>>(),std::vector<bone>());
  return *this;
}
