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

template<typename T> void
gx::rawAttrib<T>::putProcessedData(std::string shID, std::vector<T> newData) {
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

template<typename T>
gx::rawAttrib<T> gx::makeRawAttrib(std::string name, std::vector<T> data) {
  return rawAttrib<T>(std::move(name),std::move(data));
}

template<typename T> std::shared_ptr<gx::rawAttrib<T>>
gx::makeRawAttribPtr(std::string name, std::vector<T> data) {
  return std::make_shared<rawAttrib<T>>(std::move(name),std::move(data));
}

gx::graphicsEntity::graphicsEntity(std::vector<vector4f> pos,
       std::vector<vector3f> norm, std::vector<vector2f> coords,
       std::vector<vector4i> bIDs, std::vector<vector4f> bWeights,
       std::vector<GLuint>   inds, std::map<int,matrix>  offs, material m,
       std::vector<double> durs, bone bn, matrix fixMat)
  :   positions(makeRawAttribPtr("position"    ,std::move(pos))),
        normals(makeRawAttribPtr("normal"      ,std::move(norm))),
  diffuseCoords(makeRawAttribPtr("diffuseCoord",std::move(coords))),
        boneIDs(makeRawAttribPtr("boneIDs"     ,std::move(bIDs))),
    boneWeights(makeRawAttribPtr("boneWeights" ,std::move(bWeights))),
    indices(std::move(inds)), offsets(std::move(offs)), mat(std::move(m)),
    animationDurations(std::move(durs)), rootBone(std::move(bn)), 
    centerAndResize(std::move(fixMat)) {}

gx::graphicsEntity::graphicsEntity(rawAttribPtr_t<vector4f>::t pos,
    rawAttribPtr_t<vector3f>::t norms, rawAttribPtr_t<vector2f>::t coords,
    rawAttribPtr_t<vector4i>::t bIDs,  rawAttribPtr_t<vector4f>::t bWts,
    std::vector<GLuint> inds, std::map<int,matrix> offs, material m,
    std::vector<double> durs,  bone bn, matrix car)
  : positions(std::move(pos)), normals(std::move(norms)),
    diffuseCoords(std::move(coords)), boneIDs(std::move(bIDs)),
    boneWeights(std::move(bWts)), indices(std::move(inds)),
    offsets(std::move(offs)), mat(std::move(m)), 
    animationDurations(std::move(durs)), rootBone(std::move(bn)),
    centerAndResize(std::move(car)) {}

gx::graphicsEntity::graphicsEntity(graphicsEntity&& other) noexcept
  : positions(std::move(other.positions)), normals(std::move(other.normals)),
    diffuseCoords(std::move(other.diffuseCoords)),
    boneIDs(std::move(other.boneIDs)),boneWeights(std::move(other.boneWeights)),
    indices(std::move(other.indices)), offsets(std::move(other.offsets)),
    mat(std::move(other.mat)),
    animationDurations(std::move(other.animationDurations)),
    rootBone(std::move(other.rootBone)),
    centerAndResize(std::move(other.centerAndResize)) {}

gx::graphicsEntity& gx::graphicsEntity::operator=(graphicsEntity&& other) {
  this->positions         = std::move(other.positions);
  this->normals           = std::move(other.normals);
  this->diffuseCoords     = std::move(other.diffuseCoords);
  this->boneIDs           = std::move(other.boneIDs);
  this->boneWeights       = std::move(other.boneWeights);
  this->indices           = std::move(other.indices);
  this->offsets           = std::move(other.offsets);
  this->mat               = std::move(other.mat);
  this->animationDurations= std::move(other.animationDurations);
  this->rootBone          = std::move(other.rootBone);
  this->centerAndResize   = std::move(other.centerAndResize);
  return *this;
}

gx::graphicsEntity::attribsList_t
gx::graphicsEntity::getAttribList(const std::string& shID) const {
  attribsList_t ret;
  ret.push_back(this->positions    ->getProcessedData(shID));
  ret.push_back(this->normals      ->getProcessedData(shID));
  ret.push_back(this->diffuseCoords->getProcessedData(shID));
  ret.push_back(this->boneIDs      ->getProcessedData(shID));
  ret.push_back(this->boneWeights  ->getProcessedData(shID));
  return ret;
}

template class gx::rawAttrib<gx::vector4f>;
template gx::rawAttrib<gx::vector4f>
gx::makeRawAttrib(std::string name, std::vector<gx::vector4f> data);
template std::shared_ptr<gx::rawAttrib<gx::vector4f>>
gx::makeRawAttribPtr(std::string, std::vector<gx::vector4f>);
template class gx::rawAttrib<gx::vector3f>;
template gx::rawAttrib<gx::vector3f>
gx::makeRawAttrib(std::string name, std::vector<gx::vector3f> data);
template std::shared_ptr<gx::rawAttrib<gx::vector3f>>
gx::makeRawAttribPtr(std::string, std::vector<gx::vector3f>);
template class gx::rawAttrib<gx::vector2f>;
template gx::rawAttrib<gx::vector2f>
gx::makeRawAttrib(std::string name, std::vector<gx::vector2f> data);
template std::shared_ptr<gx::rawAttrib<gx::vector2f>>
gx::makeRawAttribPtr(std::string, std::vector<gx::vector2f>);
template class gx::rawAttrib<gx::vector1f>;
template gx::rawAttrib<gx::vector1f>
 gx::makeRawAttrib(std::string name, std::vector<gx::vector1f> data);
template std::shared_ptr<gx::rawAttrib<gx::vector1f>>
gx::makeRawAttribPtr(std::string, std::vector<gx::vector1f>);
template class gx::rawAttrib<gx::vector4i>;
template gx::rawAttrib<gx::vector4i>
gx::makeRawAttrib(std::string name, std::vector<gx::vector4i> data);
template std::shared_ptr<gx::rawAttrib<gx::vector4i>>
gx::makeRawAttribPtr(std::string, std::vector<gx::vector4i>);
