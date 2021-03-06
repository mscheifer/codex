#include "drawerImpl.h"
#include "graphicsEntity.h"
#include "animator.h"

namespace {
gx::graphicsEntity::attribsList_t processAttribs(const gx::graphicsEntity& gentity) {
  const std::string& shaderID = gx::dynamicDrawerImpl::shaderID;
  if(!gentity.positions->hasProcessedData(shaderID)) {
    auto rawData     = gentity.positions->getRawData();
    auto boneIDs     = gentity.boneIDs->getRawData();
    auto boneWeights = gentity.boneWeights->getRawData();
    for(unsigned int i = 0; i < rawData.size(); i++) {
      gx::matrix offset; //0 init
      typedef decltype(boneIDs) boneType;
      for(unsigned int j = 0; j < boneType::value_type::numComponents; j++) {
        if(gentity.offsets.find(boneIDs[i][j]) != gentity.offsets.end()) {
          //std::cout << "offset id " << boneIDs[i][j] << std::endl;
          offset += gentity.offsets.find(boneIDs[i][j])->second * boneWeights[i][j];
        }
      }
      rawData[i] = offset * rawData[i];
    }
    gentity.positions->putProcessedData(shaderID,rawData);
  }
  if(!gentity.normals->hasProcessedData(shaderID)) {
    auto rawData     = gentity.normals->getRawData();
    auto boneIDs     = gentity.boneIDs->getRawData();
    auto boneWeights = gentity.boneWeights->getRawData();
    for(unsigned int i = 0; i < rawData.size(); i++) {
      gx::matrix offset; //0 init
      typedef decltype(boneIDs) boneType;
      for(unsigned int j = 0; j < boneType::value_type::numComponents; j++) {
        if(gentity.offsets.find(boneIDs[i][j]) != gentity.offsets.end()) {
          offset += gentity.offsets.find(boneIDs[i][j])->second * boneWeights[i][j];
        }
      }
      //this works because we're not doing non-uniform scaling
      gx::matrix mat(0,0,1,0, //HACK TO USE LATER
                     0,1,0,0,
                     1,0,0,0,
                     0,0,0,1);
      rawData[i] = offset * rawData[i];
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

const std::string gx::dynamicDrawerImpl::shaderID = "dynamic";

const std::string gx::dynamicDrawerImpl::vertShader =
  readFile("shaders/animated.vert");
const std::string gx::dynamicDrawerImpl::fragShader =
  readFile("shaders/default.frag");

gx::dynamicDrawerImpl::entityClass::entityClass(entity_t drawData,
                                                varSigs_t vars)
  : animationDurations(std::move(drawData.animationDurations)),
    vertData(std::move(drawData.indices),processAttribs(drawData),
             std::move(vars)), mat(std::move(drawData.mat)),
    rootBone(std::move(drawData.rootBone)) {
  this->rootBone.transform = std::move(drawData.centerAndResize) *
    this->rootBone.transform;
}

gx::dynamicDrawerImpl::entityClass::entityClass(entityClass&& other) noexcept
  : instances(std::move(other.instances)), 
    staticInstances(std::move(other.staticInstances)),
    animationDurations(std::move(other.animationDurations)),
    vertData(std::move(other.vertData)),
    mat(std::move(other.mat)), rootBone(std::move(other.rootBone)) {}

gx::dynamicDrawerImpl::entityClass&
gx::dynamicDrawerImpl::entityClass::operator=(entityClass&&) {
  exit(-1); //fail hard
  return *this;
}

void gx::dynamicDrawerImpl::entityClass::clear() {
  this->instances.clear();
}

void gx::dynamicDrawerImpl::entityClass::clearStatic() {
  this->staticInstances.clear();
}

void gx::dynamicDrawerImpl::entityClass::update(displaySet const&) {
  //do nothing
}

void gx::dynamicDrawerImpl::entityClass::draw() {
  this->vertData.drawInstance();
}

gx::dynamicDrawerImpl::dynamicDrawerImpl(const shaderProgram& program)
  : staticBase(program), boneTransforms(program,"boneTransformations",maxBones) 
  {}

void gx::dynamicDrawerImpl::setUniforms(const entityClass& entC,
                                        const entityClass::instance& inst)const{
  this->staticBase.setUniforms(inst.position);
  const auto& bones = entC.rootBone.getBonesData(inst.animation,inst.timePos);
  this->boneTransforms.write(bones.data(),
                             static_cast<GLsizei>(entC.rootBone.numBones()));
}

void gx::dynamicDrawerImpl::addInstance(
    const instanceData& d,std::vector<entityClass>& entityClasses) {
  auto type = d.type;
  entityClass::instance newInst;
  newInst.animation = d.animation;
  newInst.  timePos = d.timePosition / animator::animationLength * 
                      entityClasses[type].animationDurations[d.animation];
  newInst. position = staticDrawerImpl::makePositionMatrix(std::move(d));

  entityClasses[type].instances.push_back(std::move(newInst));
}

void gx::dynamicDrawerImpl::addStaticInstance(
    const instanceData& d,std::vector<entityClass>& entityClasses) {
  auto type = d.type;
  entityClass::instance newInst;
  newInst.animation = d.animation;
  newInst.  timePos = d.timePosition;
  newInst. position = staticDrawerImpl::makePositionMatrix(d);

  entityClasses[type].staticInstances.push_back(std::move(newInst));
}