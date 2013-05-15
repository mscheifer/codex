#include "drawSet.h"
#include "displaySet.h"
#include "uniformBlock.h"

gx::drawSet::entityClass::entityClass(std::vector<matrix> poses, vao vaostuff)
  : positions(std::move(poses)), vertData(std::move(vaostuff)) {}

gx::drawSet::entityClass::entityClass(entityClass&& other)
  : positions(std::move(other.positions)), vertData(std::move(other.vertData)) {}

gx::drawSet::drawSet(const std::string vertShader, const std::string fragShader,
                     const std::vector<vaoData_t> vaoDatas,
         std::vector<uniform::block*>  globalUnifs)
  : program(vertShader, fragShader, globalUnifs), entityClasses(),
    modelToWorldLoc(program,"modelToWorld",1), globalUniforms(globalUnifs) {
  for(auto vaoDatap = vaoDatas.begin(); vaoDatap != vaoDatas.end(); ++vaoDatap){
    const auto& vaoData = *vaoDatap;
    entityClass newEntClass(std::vector<matrix>(),
                  vao(vaoData.first,vaoData.second,program.vars()));
    entityClasses.push_back(std::move(newEntClass));
  }
}

void gx::drawSet::draw() const {
  this->program.use();
  for(auto unifP = this->globalUniforms.begin(); unifP != this->globalUniforms.end(); unifP++) {
    (*unifP)->frameUpdate(&(this->program));
  }
  for(auto entityCp = entityClasses.begin(); entityCp != entityClasses.end();
                                                                 ++entityCp) {
    const auto& entityC = *entityCp;
    for(auto locp = entityC.positions.begin(); locp != entityC.positions.end(); 
                                                                       ++locp) {
      const auto& loc = *locp;
      this->modelToWorldLoc.write(loc.oglmatrix().data());
      entityC.vertData.draw();
    }
  }
}

void gx::drawSet::reset() {
  for(auto entityCp = entityClasses.begin(); entityCp != entityClasses.end();
                                                                 ++entityCp) {
    auto& entityC = *entityCp;
    entityC.positions.clear();
  }
}

void gx::drawSet::addEntity(vector3f pos,vector3f dirY,unsigned int type) {
  dirY.z = 0;
  dirY.normalize();
  const vector3f up = vector3f(0,0,1);
  matrix rotAndTrans = translation(pos.x,pos.y,pos.z) * toRightHandBasisFromYandUp(dirY,up);
  this->entityClasses[type].positions.push_back(rotAndTrans);
}
