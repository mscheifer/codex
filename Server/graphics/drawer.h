#ifndef DRAWER_H
#define DRAWER_H
#include <vector>
#include "shaderProgram.h"
#include "drawerImpl.h"
#include "particleSystem.h"

namespace gx {

template<typename T>
class drawer {
                       const displaySet& camData;
                           shaderProgram program;
                                       T impl;
    std::vector<typename T::entityClass> entityClasses;
            std::vector<uniform::block*> globalUniforms;
  public:
    drawer(displaySet const&,std::vector<typename T::entity_t>, std::vector<uniform::block*>);
    void draw();
    void reset();
	  void resetStatic();
    typedef typename T::instanceData instanceData;
    void addInstance(instanceData const&);
    void addStaticInstance(instanceData const&);
};

typedef  drawer<staticDrawerImpl> staticDrawer;
typedef drawer<dynamicDrawerImpl> dynamicDrawer;
typedef drawer<particleDrawerImpl> particleDrawer;

} //end namespace gx
#endif //DRAWER_H