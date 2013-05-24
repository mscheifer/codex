#include "bone.h"

gx::matrix gx::toMat(aiMatrix4x4 m) {
  return gx::matrix(m.a1, m.a2, m.a3, m.a4,
                    m.b1, m.b2, m.b3, m.b4,
                    m.c1, m.c2, m.c3, m.c4,
                    m.d1, m.d2, m.d3, m.d4);
}

gx::matrix gx::toMat(aiMatrix3x3 m) {
  return gx::matrix(m.a1, m.a2, m.a3, 0,
                    m.b1, m.b2, m.b3, 0,
                    m.c1, m.c2, m.c3, 0,
                       0,    0,    0, 1);
}

gx::bone::bone(int idnum,matrix off, matrix trans, bool r, std::vector<std::vector<key>> anims, 
     std::vector<bone> enfants)
  : id(idnum), offset(std::move(off)), transform(std::move(trans)), real(std::move(r)),
    animations(std::move(anims)), children(std::move(enfants)) {}

gx::bone::bone(bone&& other) noexcept
  : id(other.id), offset(std::move(other.offset)),
  transform(std::move(other.transform)), real(std::move(other.real)), 
  animations(std::move(other.animations)), children(std::move(other.children)){}

gx::bone& gx::bone::operator=(bone&& other) {
  this->id         = std::move(other.id);
  this->offset     = std::move(other.offset);
  this->transform  = std::move(other.transform);
  this->real       = std::move(other.real); 
  this->animations = std::move(other.animations);
  this->children   = std::move(other.children);
  return *this;
}

bool gx::bone::animated(unsigned int index) const {
  return !this->animations[index].empty();
}

std::vector<GLfloat> gx::bone::getBonesData(unsigned int anim, unsigned int time) const {
  std::vector<GLfloat> ret;
  this->walkBones(ret,identity,anim,time);
  return ret;
}

unsigned int gx::bone::numBones() const {
  unsigned int ret = this->real ? 1 : 0;
  for(auto boneItr = this->children.begin(); boneItr != this->children.end(); boneItr++) {
    ret += boneItr->numBones();
  }
  return ret;
}

void gx::bone::walkBones(std::vector<GLfloat>& result,const matrix& parent,
                         unsigned int anim, unsigned int time) const {
  matrix fullTransformation;
  if(this->animated(anim)) {
    //no interpolation for now
    const auto& frame = this->animations[anim][time];
    const auto& scaleVec = frame.scaling.mValue;
    const auto& transVec = frame.position.mValue;
    matrix scale    = scaling    (scaleVec.x, scaleVec.y, scaleVec.z);
    matrix rotation = toMat      (frame.rotation.mValue.GetMatrix());
    matrix trans    = translation(transVec.x, transVec.y, transVec.z);
    fullTransformation = parent * trans * rotation * scale;
    //std::cout << "animating " << this->id << std::endl;
  } else {
    fullTransformation = parent * this->transform;
  }
  if(this->real) {
    const auto& data = (fullTransformation * this->offset).oglmatrix();
    //std::cout << this->id << " at " << result.size() << std::endl;
    result.insert(result.end(),data.begin(),data.end());
  } 
  for(auto boneItr = this->children.begin(); boneItr != this->children.end(); boneItr++) {
    boneItr->walkBones(result,fullTransformation,anim,time);
  }
}
