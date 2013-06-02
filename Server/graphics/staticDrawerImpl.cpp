#include "drawerImpl.h"
#include <assert.h>
#include "graphicsEntity.h"
#include "uniformBlock.h"
#include "util.h"

gx::matrix gx::staticDrawerImpl::makePositionMatrix(instanceData d) {
  d.dirY.z = 0;
  d.dirY.normalize();
  const gx::vector3f up  = gx::vector3f(0,0,1);
  gx::matrix rotAndTrans = gx::translation(d.pos.x,d.pos.y,d.pos.z) *
                           gx::toRightHandBasisFromYandUp(d.dirY,up) *
                           uniformScaling(d.scale);
  return rotAndTrans;
}

const std::string gx::staticDrawerImpl::shaderID = "static";

const std::string gx::staticDrawerImpl::vertShader =
  readFile("shaders/default.vert");
const std::string gx::staticDrawerImpl::fragShader =
  readFile("shaders/default.frag");

namespace {
gx::graphicsEntity::attribsList_t processAttribs(const gx::graphicsEntity& gentity) {
  const std::string& shaderID = gx::staticDrawerImpl::shaderID;
  if(!gentity.positions->hasProcessedData(shaderID)) {
    auto rawData = gentity.positions->getRawData();
    for(auto itr = rawData.begin(); itr != rawData.end(); itr++) {
      *itr = gentity.centerAndResize * (*itr);
    }
    gentity.positions->putProcessedData(shaderID,rawData);
  }
  if(!gentity.normals->hasProcessedData(shaderID)) {
    auto rawData = gentity.normals->getRawData();
    for(auto itr = rawData.begin(); itr != rawData.end(); itr++) {
      //this works because we're not doing non-uniform scaling
      *itr = gentity.centerAndResize * (*itr);
    }
    gentity.normals->putProcessedData(shaderID,rawData);
  }
  if(!gentity.diffuseCoords->hasProcessedData(shaderID)) {
    gentity.diffuseCoords->putDefaultData(shaderID);
  }
  if(!gentity.boneIDs->hasProcessedData(shaderID)) {
    gentity.boneIDs->putDefaultData(shaderID);
  }
  if(!gentity.boneWeights->hasProcessedData(shaderID)) {
    gentity.boneWeights->putDefaultData(shaderID);
  }
  return gentity.getAttribList(shaderID);
}
} //end unnamed namespace

gx::staticDrawerImpl::entityClass::entityClass(graphicsEntity drawData,
                                               varSigs_t vars)
  : vertData(std::move(drawData.indices), processAttribs(drawData),
    std::move(vars)), mat(std::move(drawData.mat)) {}

gx::staticDrawerImpl::entityClass::entityClass(entityClass&& other) noexcept
  : instances(std::move(other.instances)), vertData(std::move(other.vertData)),
    mat(std::move(other.mat)) {}

gx::staticDrawerImpl::entityClass&
gx::staticDrawerImpl::entityClass::operator=(entityClass&&) {
  assert(false);
  return *this;
}

void gx::staticDrawerImpl::entityClass::clear() {
  this->instances.clear();
}

gx::staticDrawerImpl::staticDrawerImpl(const shaderProgram& program)
  : modelToWorldLoc(program,"modelToWorld",1) {}

void gx::staticDrawerImpl::setUniforms(const entityClass& entC,
                                       const entityClass::instance& inst) const{
  this->modelToWorldLoc.write(inst.oglmatrix().data());
}

void gx::staticDrawerImpl::setUniforms(const entityClass::instance& inst) const{
  this->modelToWorldLoc.write(inst.oglmatrix().data());
}

void gx::staticDrawerImpl::addInstance(
    instanceData d,std::vector<entityClass>& entityClasses) {
  auto type = d.type;
  entityClasses[type].instances.push_back(makePositionMatrix(std::move(d)));
}

