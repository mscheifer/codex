#include "graphicsEntity.h"
#include "vector1.h"

template<typename T>
gx::rawAttrib<T>::rawAttrib(std::string n,std::vector<T> datas)
  : name(std::move(n)), data(std::move(datas)) {}

template<typename T>
std::vector<T> gx::rawAttrib<T>::getRawData() const {
  return this->data;
}

template<typename T>
bool gx::rawAttrib<T>::hasProcessedData(const std::string& shID) const {
  return this->processedAttribs.count(shID) > 0;
}

template<typename T>
std::shared_ptr<gx::vertexAttrib>
gx::rawAttrib<T>::getProcessedData(const std::string& shID) const {
  //copying increases the reference count
  return this->processedAttribs.find(shID)->second;
}

template<typename T>
void gx::rawAttrib<T>::putProcessedData(std::string shID, std::vector<T> newData) {
  std::vector<typename T::elem_t> flatData;
  for(auto itr = newData.begin(); itr != newData.end(); itr++) {
    for(auto vitr = itr->begin(); vitr != itr->end(); vitr++) {
      flatData.push_back(*vitr);
    }
  }
  auto attrib =
    std::make_shared<gx::vertexAttrib>(this->name,T::numComponents,0,flatData);
  this->processedAttribs.insert(
    std::make_pair(std::move(shID),std::move(attrib)));
}

template<typename T>
void gx::rawAttrib<T>::putDefaultData(std::string shID) {
  this->putProcessedData(std::move(shID),this->data);
}

gx::graphicsEntity::graphicsEntity(std::vector<vector4f> pos,
       std::vector<vector3f> norm, std::vector<vector4f> cols,
       std::vector<vector4i> bIDs, std::vector<vector4f> bWeights,
       std::vector<GLuint>   inds, std::map<int,matrix>  offs, bone bn,
       matrix fixMat)
  :   positions(makeRawAttribPtr("position"   ,pos)),
        normals(makeRawAttribPtr("normal"     ,norm)),
         colors(makeRawAttribPtr("color"      ,cols)),
        boneIDs(makeRawAttribPtr("boneIDs"    ,bIDs)),
    boneWeights(makeRawAttribPtr("boneWeights",bWeights)),
    indices(std::move(inds)), offsets(std::move(offs)), rootBone(std::move(bn)),
    centerAndResize(std::move(fixMat)) {}

gx::graphicsEntity::graphicsEntity(rawAttribPtr_t<vector4f> pos,
    rawAttribPtr_t<vector3f> norms, rawAttribPtr_t<vector4f> cols,
    rawAttribPtr_t<vector4i> bIDs, rawAttribPtr_t<vector4f> bWts,
    std::vector<GLuint> inds, std::map<int,matrix> offs, bone bn, matrix car)
  : positions(std::move(pos)), normals(std::move(norms)), colors(std::move(cols)),
    boneIDs(std::move(bIDs)), boneWeights(std::move(bWts)),
    indices(std::move(inds)), offsets(std::move(offs)), rootBone(std::move(bn)),
    centerAndResize(std::move(car)) {}

gx::graphicsEntity::attribsList_t gx::graphicsEntity::getAttribList(const std::string& shID) const {
  attribsList_t ret;
  ret.push_back(this->positions  ->getProcessedData(shID));
  ret.push_back(this->normals    ->getProcessedData(shID));
  ret.push_back(this->colors     ->getProcessedData(shID));
  ret.push_back(this->boneIDs    ->getProcessedData(shID));
  ret.push_back(this->boneWeights->getProcessedData(shID));
  return ret;
}

template class gx::rawAttrib<gx::vector4f>;
template class gx::rawAttrib<gx::vector3f>;
template class gx::rawAttrib<gx::vector4i>;
template class gx::rawAttrib<gx::vector1f>;
