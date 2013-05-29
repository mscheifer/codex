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
gx::drawer<T>::drawer(std::vector<typename T::entity_t> entDatas,
                      std::vector<uniform::block*> globalUnifs)
  : program(T::vertShader, T::fragShader, globalUnifs), impl(program), 
    entityClasses(setupEntities<T>(std::move(entDatas),program.vars())),
    globalUniforms(globalUnifs) {
  GLint diffuseLoc = program.uniformLoc("diffuseTex");
  this->program.use();
  glUniform1i(diffuseLoc, 0); //TODO: change 0 to constant that corresponds with GL_TEXTURE0
  debugout << "glUniform1i(" << diffuseLoc << ", 2);" << endl;
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
    const auto& entityC = *entityCp;
    entityC.mat.bind();
    for(auto instp = entityC.instances.begin();instp != entityC.instances.end();
                                                                      ++instp) {
      const auto& inst = *instp;
      this->impl.setUniforms(entityC,inst);
      entityC.vertData.draw();
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
void gx::drawer<T>::addInstance(instanceData a) {
  impl.addInstance(std::move(a),entityClasses);
}

template class gx::drawer<gx::staticDrawerImpl>;
template class gx::drawer<gx::dynamicDrawerImpl>;
