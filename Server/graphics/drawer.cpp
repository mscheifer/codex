#include "drawer.h"
#include "graphicsEntity.h"
#include "uniformBlock.h"

namespace {
template<typename T>
std::vector<typename T::entityClass> setupEntities(
    std::vector<typename T::entity_t> entDatas,
    std::map<std::string,gx::vertexAttribSignature> vars) {
  std::vector<typename T::entityClass> entityClasses;
  for(auto entDatap = entDatas.begin(); entDatap != entDatas.end(); ++entDatap){
    auto& entData = *entDatap;
    entityClasses.push_back(typename T::entityClass(std::move(entData),vars));
  }
  return entityClasses;
}
} //end unnamed namespace

template<typename T>
gx::drawer<T>::drawer(displaySet const& ds,
                      std::vector<typename T::entity_t> entDatas,
                      std::vector<uniform::block*> globalUnifs)
  : camData(ds), program(T::vertShader, T::fragShader, globalUnifs),
    impl(program), 
    entityClasses(setupEntities<T>(std::move(entDatas),program.vars())),
    globalUniforms(globalUnifs) {
  material::setupBindings(this->program);
}

template<typename T>
void gx::drawer<T>::draw() {
  this->program.use();
  for(auto unifP  = this->globalUniforms.begin();
           unifP != this->globalUniforms.end(); unifP++) {
    (*unifP)->frameUpdate(&(this->program));
  }
  for(auto entityCp = entityClasses.begin(); entityCp != entityClasses.end();
                                                                 ++entityCp) {
    auto& entityC = *entityCp;
    entityC.update(this->camData);
    entityC.mat.bind();
    entityC.vertData.drawHead();
    for(auto instp = entityC.instances.begin();instp != entityC.instances.end();
                                                                      ++instp) {
      const auto& inst = *instp;
      this->impl.setUniforms(entityC,inst);
      entityC.draw();
    }
    for(auto instp  = entityC.staticInstances.begin();
             instp != entityC.staticInstances.end();   ++instp) {
      const auto& inst = *instp;
      this->impl.setUniforms(entityC,inst);
      entityC.draw();
    }
  }
}

template<typename T>
void gx::drawer<T>::reset() {
  for(auto entityCp = entityClasses.begin(); entityCp != entityClasses.end();
                                                                 ++entityCp) {
    auto& entityC = *entityCp;
    entityC.clear();
  }
}

template<typename T>
void gx::drawer<T>::resetStatic() {
  for(auto entityCp = entityClasses.begin(); entityCp != entityClasses.end();
                                                                 ++entityCp) {
    auto& entityC = *entityCp;
    entityC.clearStatic();
  }
}

template<typename T>
void gx::drawer<T>::addInstance(const instanceData& a) {
  impl.addInstance(a,entityClasses);
}

template<typename T>
void gx::drawer<T>::addStaticInstance(const instanceData& a) {
  impl.addStaticInstance(a,entityClasses);
}

template class gx::drawer<gx::staticDrawerImpl>;
template class gx::drawer<gx::dynamicDrawerImpl>;
template class gx::drawer<gx::particleDrawerImpl>;