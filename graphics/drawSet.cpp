#include "drawSet.h"

gx::drawSet::drawSet(const std::string vertShader, const std::string fragShader,
                     const std::vector<vaoData_t> vaoDatas,
                     std::vector<const uniform*>  globalUniforms)
  : instancePos("trans",sizeof(GLfloat[16])), //fix this initialization
    program(vertShader, fragShader, (globalUniforms.push_back(&instancePos),
    globalUniforms)), entityClasses() {
  for(auto vaoDatap = vaoDatas.begin(); vaoDatap != vaoDatas.end(); ++vaoDatap){
    const auto& vaoData = *vaoDatap;
	entityClass newEntClass = { std::vector<matrix>(),
                  vao(vaoData.first,vaoData.second,program.vars()) };
    entityClasses.push_back(newEntClass);
  }
  //setup instancePos
}

void gx::drawSet::draw() const {
  this->program.use();
  for(auto entityCp = entityClasses.begin(); entityCp != entityClasses.end(); ++entityCp){
    const auto& entityC = * entityCp;
	for(auto locp = entityC.positions.begin(); locp != entityC.positions.end(); ++locp){
      const auto& loc = *locp;
      this->instancePos.write(0,loc.oglmatrix());
      entityC.vertData.draw();
    }
  }
}

void gx::drawSet::addEntity(vector3 pos,unsigned int type) {
  this->entityClasses[type].positions.push_back(translation(pos.x,pos.y,pos.z));
}
