#include "graphicsEntity.h"

gx::staticEntity::staticEntity(std::vector<GLuint> ind, attribsList_t attr)
  : indices(ind), attribs(attr) {}

gx::staticEntity::staticEntity(staticEntity&& other) noexcept
  : indices(std::move(other.indices)), attribs(std::move(other.attribs)) {}

gx::staticEntity& gx::staticEntity::operator=(staticEntity other) {
  this->indices = std::move(other.indices);
  this->attribs = std::move(other.attribs);
  return *this;
}

gx::dynamicEntity::dynamicEntity(std::vector<GLuint> indices, attribsList_t attribs)
  : staticEntity(std::move(indices),std::move(attribs)) {}

gx::dynamicEntity::dynamicEntity(dynamicEntity&& other) noexcept
  : staticEntity(std::move(other)) {}

gx::dynamicEntity& gx::dynamicEntity::operator=(dynamicEntity&& other) {
  staticEntity::operator=(std::move(other));
  return *this;
}

gx::dynamicEntity::dynamicEntity(staticEntity&& other): staticEntity(std::move(other)) {}

gx::dynamicEntity& gx::dynamicEntity::operator=(staticEntity&& other) {
  staticEntity::operator=(std::move(other));
  return *this;
}
