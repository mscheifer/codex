#ifndef DRAWER_H
#define DRAWER_H
#include <vector>
#include "shaderProgram.h"
#include "graphicsEntity.h"
#include "drawSet.h"

namespace gx {

template<typename T>
class drawer {
                           shaderProgram program;
                                       T impl;
    std::vector<typename T::entityClass> entityClasses;
	          std::vector<uniform::block*> globalUniforms;
  public:
    drawer(std::vector<dynamicEntity>, std::vector<uniform::block*>);
    void draw();
    void reset();
    typedef typename T::instanceData instanceData;
    void addInstance(instanceData);
};

typedef  drawer<staticDrawerImpl> staticDrawer;
//typedef drawer<dynamicDrawerImpl> dynamicDrawer;

} //end namespace gx
#endif //DRAWER_H
