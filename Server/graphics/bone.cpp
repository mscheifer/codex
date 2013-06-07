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

gx::bone::bone(int idnum, matrix trans, bool r,
               decltype(animations) anims, std::vector<bone> enfants)
  : id(idnum), transform(std::move(trans)), real(std::move(r)),
    animations(std::move(anims)), children(std::move(enfants)) {}

gx::bone::bone(bone&& other) noexcept
  : id(other.id), transform(std::move(other.transform)), real(other.real), 
  animations(std::move(other.animations)), children(std::move(other.children)){}

gx::bone& gx::bone::operator=(bone&& other) {
  this->id         = std::move(other.id);
  this->transform  = std::move(other.transform);
  this->real       = std::move(other.real); 
  this->animations = std::move(other.animations);
  this->children   = std::move(other.children);
  return *this;
}

bool gx::bone::animated(unsigned int index) const {
  return index < this->animations.size() && 
	    !this->animations[index].keyFrames.empty();
}

std::vector<GLfloat>
gx::bone::getBonesData(unsigned int anim, double time) const {
  std::vector<GLfloat> ret;
  this->walkBones(ret,identity,anim,time);
  return ret;
}

unsigned int gx::bone::numBones() const {
  unsigned int ret = this->real ? 1 : 0;
  for(auto boneItr = this->children.begin(); boneItr != this->children.end();
                                                                  boneItr++) {
    ret += boneItr->numBones();
  }
  return ret;
}

namespace {
  double interpolationFactor(double time, double lowerTime, double upperTime) {
    double deltaTime = upperTime - lowerTime;
    double factor = (time - lowerTime) / deltaTime;
    assert(factor >= 0.0 && factor <= 1.0);
    return factor;
  }

  gx::vector3f interpolatedScaling(double factor, const gx::bone::key& lower, const gx::bone::key& upper) {
    auto change = upper.scaling - lower.scaling;
    return lower.scaling + static_cast<float>(factor) * change;
  }

  gx::vector4f interpolatedTranslation(double factor, const gx::bone::key& lower, const gx::bone::key& upper) {
    auto change = upper.position - lower.position;
    return lower.position + static_cast<float>(factor) * change;
  }

  aiQuaternion interpolatedRotation(double factor, const gx::bone::key& lower, const gx::bone::key& upper) {
    aiQuaternion ret;
    aiQuaternion::Interpolate(ret,lower.rotation,upper.rotation, static_cast<float>(factor));
    ret.Normalize();
    return ret;
  }
} //end unnamed namespace

void gx::bone::walkBones(std::vector<GLfloat>& result,const matrix& parent,
                         unsigned int anim, double time) const {
  matrix fullTransformation;
  if(this->animated(anim)) {
    //no interpolation for now
    const auto& animation = this->animations[anim];
    const auto& keyFrames = animation.keyFrames;
    auto lowerBound = keyFrames.lower_bound(time);
    auto upperBound = keyFrames.upper_bound(time);
    assert(lowerBound->first <= time || lowerBound != keyFrames.begin());
    if(lowerBound->first > time) --lowerBound;
    assert(upperBound != keyFrames.end());
    assert(lowerBound != upperBound);
    const auto& lowerFrame = lowerBound->second;
    const auto& upperFrame = upperBound->second;
    double factor = interpolationFactor(time,lowerBound->first,upperBound->first);
    const auto& scaleVec = interpolatedScaling(factor,lowerFrame,upperFrame);
    const auto& transVec = interpolatedTranslation(factor,lowerFrame,upperFrame);
    const auto& quat = interpolatedRotation(factor,lowerFrame,upperFrame);
    matrix scale    = scaling    (scaleVec.x, scaleVec.y, scaleVec.z);
    matrix rotation = toMat      (quat.GetMatrix());
    matrix trans    = translation(transVec.x, transVec.y, transVec.z);
    fullTransformation = parent * trans * rotation * scale;
    //std::cout << "animating " << this->id << std::endl;
  } else {
    fullTransformation = parent * this->transform;
  }
  if(this->real) {
    //const auto& data = identity.oglmatrix();
    const auto& data = fullTransformation.oglmatrix();
    result.insert(result.end(),data.begin(),data.end());
  } 
  for(auto boneItr = this->children.begin(); boneItr != this->children.end();
                                                                  boneItr++) {
    boneItr->walkBones(result,fullTransformation,anim,time);
  }
}
