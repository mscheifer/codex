#ifndef BONE_H
#define BONE_H
#include <memory>
#include <assimp/scene.h>			    // Output data structure
#include "matrix.h"

namespace gx {

gx::matrix toMat(aiMatrix4x4);

gx::matrix toMat(aiMatrix3x3);

struct bone {
  int id;
  matrix offset; //applied before everything else, in addition to interpolated
                 //transformations
                 //assume it's the same for each mesh for now, if we do more
                 //than 1 mesh
  matrix transform; //applied if there is no animation active/present
  struct key {
    //assimp stuff
    aiVectorKey position;
    aiQuatKey   rotation;
    aiVectorKey scaling;
  };
  //whether any verts have this bone or not
  bool                          real; //could change this to id == -1
  std::vector<std::vector<key>> animations;
  std::vector<bone>             children;

  bone(int,matrix,matrix,bool,std::vector<std::vector<key>>,std::vector<bone>);
  bone(const bone&);// = delete;
  bone& operator=(const bone&);// = delete;
  bone(bone&&);
  bone& operator=(bone&&);

  bool animated(unsigned int) const;

  //could probably be optimized to not use heap
  //when we add interpolation, time will need to be handled differently
  //  we have to get it from the key and compare somehow
  std::vector<GLfloat> getBonesData(unsigned int,unsigned int) const;
  //the number of bones to be actually written to shader, should be same as 
  //getBonesData().size() / 16
          unsigned int numBones() const;
  private:
    void walkBones(std::vector<GLfloat>&,const matrix&,unsigned int,unsigned int) const;
};

} //end namespace gx
#endif //BONE_H
