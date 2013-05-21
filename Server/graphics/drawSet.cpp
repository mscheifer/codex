#include "drawSet.h"
#include "uniformBlock.h"
#include "util.h"

const std::string gx::staticDrawerImpl::vertShader = readFile("shaders/default.vert");
const std::string gx::staticDrawerImpl::fragShader = readFile("shaders/default.frag");

gx::staticDrawerImpl::entityClass::entityClass(staticEntity drawData,
                std::map<std::string,vertexAttribSignature> vars)
  : vertData(std::move(drawData.indices),std::move(drawData.attribs),std::move(vars)) {}

gx::staticDrawerImpl::entityClass::entityClass(entityClass&& other)
  : instances(std::move(other.instances)), vertData(std::move(other.vertData)) {}

void gx::staticDrawerImpl::entityClass::clear() {
  this->instances.clear();
}

gx::staticDrawerImpl::staticDrawerImpl(const shaderProgram& program)
  : modelToWorldLoc(program,"modelToWorld",1) {}

void gx::staticDrawerImpl::setUniforms(const entityClass::instance& inst) const {
  this->modelToWorldLoc.write(inst.oglmatrix().data());
}

void gx::staticDrawerImpl::addInstance(instanceData d,std::vector<entityClass>& entityClasses) {
  d.dirY.z = 0;
  d.dirY.normalize();
  const vector3f up = vector3f(0,0,1);
  matrix rotAndTrans = translation(d.pos.x,d.pos.y,d.pos.z) * toRightHandBasisFromYandUp(d.dirY,up);
  entityClasses[d.type].instances.push_back(rotAndTrans);
}
