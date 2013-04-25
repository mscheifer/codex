#include "drawSet.h"

gx::drawSet::drawSet(const std::string vertShader, const std::string fragShader,
                     const std::vector<vaoData_t> vaoDatas,
                     std::vector<const uniform*>  globalUniforms)
  : instancePos("trans",sizeof(GLfloat[16])), //fix this initialization
    program(vertShader, fragShader, (globalUniforms.push_back(&instancePos),
    globalUniforms)), entityClasses() {
  for(const auto& vaoData : vaoDatas) {
    entityClasses.push_back({ std::vector<matrix>(),
                  vao(vaoData.first,vaoData.second,program.vars()) });
  }
  //setup instancePos
}

void gx::drawSet::draw() const {
  this->program.use();
  for(const auto& entityC : entityClasses) {
    for(const auto& loc     : entityC.positions) {
      this->instancePos.write(0,loc.oglmatrix());
      entityC.vertData.draw();
    }
  }
}

void gx::drawSet::addEntity(vector3 pos,unsigned int type) {
  this->entityClasses[type].positions.push_back(translation(pos.x,pos.y,pos.z));
}
