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
    modelToWorldLoc(program.uniformLoc("modelToWorld")), globalUniforms(globalUnifs) {
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
      glUniformMatrix4fv(this->modelToWorldLoc,1,false,loc.oglmatrix().data());
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

void gx::drawSet::addEntity(vector4f pos,vector3f dirY,unsigned int type) {
  /*
  vector3f dirX(dirY.y, -dirY.x, 0);
  dirY.normalize();
  dirX.normalize();
  matrix rotAndTrans = translation(pos.x,pos.y,pos.z) * toBasis(dirX,dirY,vector3f(0,0,1));
  */
  dirY.z = 0;
  dirY.normalize();
  matrix rotAndTrans = translation(pos.x,pos.y,pos.z) * toRightHandBasisFromY(dirY);
  this->entityClasses[type].positions.push_back(rotAndTrans);
}
