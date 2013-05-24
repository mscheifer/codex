#include "drawSet.h"
#include "uniformBlock.h"
#include "util.h"

namespace {
gx::matrix makePositionMatrix(gx::staticDrawerImpl::instanceData d) {
  d.dirY.z = 0;
  d.dirY.normalize();
  const gx::vector3f up  = gx::vector3f(0,0,1);
  gx::matrix rotAndTrans = gx::translation(d.pos.x,d.pos.y,d.pos.z) *
                           gx::toRightHandBasisFromYandUp(d.dirY,up);
  return rotAndTrans;
}
} //end unnamed namespace

const std::string gx::staticDrawerImpl::vertShader =
  readFile("shaders/default.vert");
const std::string gx::staticDrawerImpl::fragShader =
  readFile("shaders/default.frag");

const std::string gx::dynamicDrawerImpl::vertShader =
  readFile("shaders/animated.vert");
const std::string gx::dynamicDrawerImpl::fragShader =
  readFile("shaders/animated.frag");

gx::staticDrawerImpl::entityClass::entityClass(staticEntity drawData,
                std::map<std::string,vertexAttribSignature> vars)
  : vertData(std::move(drawData.indices),std::move(drawData.attribs),
  std::move(vars)), centerAndResize(std::move(drawData.centerAndResize)) {}

gx::staticDrawerImpl::entityClass::entityClass(entityClass&& other) noexcept
  : instances(std::move(other.instances)), vertData(std::move(other.vertData)),
    centerAndResize(std::move(other.centerAndResize)) {}

gx::staticDrawerImpl::entityClass& gx::staticDrawerImpl::entityClass::operator=(entityClass&&) {
  *((int*) nullptr) = 1; //this is to fail hard if this is actually called
  return *this;
}

void gx::staticDrawerImpl::entityClass::clear() {
  this->instances.clear();
}

gx::staticDrawerImpl::staticDrawerImpl(const shaderProgram& program)
  : modelToWorldLoc(program,"modelToWorld",1) {}

void gx::staticDrawerImpl::setUniforms(const entityClass& entC,
                                       const entityClass::instance& inst) const{
  //see if there is a way to not write centerAndResize every frame
  this->modelToWorldLoc.write((inst * entC.centerAndResize).oglmatrix().data());
}

void gx::staticDrawerImpl::setUniforms(const entityClass::instance& inst) const{
  this->modelToWorldLoc.write(inst.oglmatrix().data());
}

void gx::staticDrawerImpl::addInstance(
    instanceData d,std::vector<entityClass>& entityClasses) {
  auto type = d.type;
  entityClasses[type].instances.push_back(makePositionMatrix(std::move(d)));
}

gx::dynamicDrawerImpl::entityClass::entityClass(dynamicEntity drawData,
                  std::map<std::string,vertexAttribSignature> vars)
  : vertData(std::move(drawData.indices),std::move(drawData.attribs),
             std::move(vars)), rootBone(std::move(drawData.rootBone)) {}

gx::dynamicDrawerImpl::entityClass::entityClass(entityClass&& other) noexcept
  : vertData(std::move(other.vertData)), rootBone(std::move(other.rootBone)) {}

gx::dynamicDrawerImpl::entityClass& gx::dynamicDrawerImpl::entityClass::operator=(entityClass&&) {
  *((int*) nullptr) = 1;
  return *this;
}

void gx::dynamicDrawerImpl::entityClass::clear() {
  this->instances.clear();
}

gx::dynamicDrawerImpl::dynamicDrawerImpl(const shaderProgram& program)
  : staticBase(program), boneTransforms(program,"boneTransformations",maxBones) {}

void gx::dynamicDrawerImpl::setUniforms(const entityClass& entC,
                                        const entityClass::instance& inst)const{
  this->staticBase.setUniforms(inst.position);
  const auto& bones = entC.rootBone.getBonesData(inst.animation,inst.timePos);
  this->boneTransforms.write(bones.data(),entC.rootBone.numBones());
}

void gx::dynamicDrawerImpl::addInstance(
    instanceData d,std::vector<entityClass>& entityClasses) {
  auto type = d.type;
  entityClass::instance newInst;
  newInst.animation = d.animation;
  newInst.  timePos = d.timePosition;
  newInst. position = makePositionMatrix(std::move(d));

  entityClasses[type].instances.push_back(std::move(newInst));
}
