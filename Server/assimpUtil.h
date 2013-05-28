#ifndef ASSIMPUTIL_H
#define ASSIMPUTIL_H
namespace gx {
vector4f toVec4(const aiVector3D& aiVec) {
  return gx::vector4f(aiVec.x,aiVec.y,aiVec.z);
}
vector4f toVec4(const aiColor3D& aiVec) {
  return gx::vector4f(aiVec.r,aiVec.b,aiVec.g);
}
vector3f toVec3(const aiVector3D& aiVec) {
  return gx::vector3f(aiVec.x,aiVec.y,aiVec.z);
}
} //end namespace gx
#endif