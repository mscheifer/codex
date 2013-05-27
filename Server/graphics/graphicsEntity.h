#ifndef GRAPHICSENTITY_H
#define GRAPHICSENTITY_H
#include <string>
#include <vector>
#include <map>
#include "vertexAttrib.h"
#include "vector4.h"
#include "vector3.h"
#include "bone.h"

namespace gx {

template<typename T>
class rawAttrib {
    std::string    name;
    std::vector<T> data;
    std::map<std::string,std::shared_ptr<vertexAttrib>> processedAttribs;
  public:
    //static constexpr unsigned int numComponents = T::numComponents;
    rawAttrib(std::string,std::vector<T>);
    std::vector<T> getRawData() const;
    bool hasProcessedData(std::string const&) const;
    std::shared_ptr<vertexAttrib> getProcessedData(std::string const&) const;
    void putProcessedData(std::string,std::vector<T>);
    void putDefaultData(std::string);
};

template<typename T>
rawAttrib<T> makeRawAttrib(std::string name, std::vector<T> data) {
  return rawAttrib<T>(std::move(name),std::move(data));
}

template<typename T>
std::shared_ptr<rawAttrib<T>> makeRawAttribPtr(std::string name, std::vector<T> data) {
  return std::make_shared<rawAttrib<T>>(std::move(name),std::move(data));
}

struct graphicsEntity {
    template<typename T>
    using rawAttribPtr_t = std::shared_ptr<rawAttrib<T>>;
    //-- this block
    rawAttribPtr_t<vector4f> positions;
    rawAttribPtr_t<vector3f> normals;
    //rawAttribPtr_t<vector2f> texcoords;
    rawAttribPtr_t<vector4f> colors;
    rawAttribPtr_t<vector4i> boneIDs;
    rawAttribPtr_t<vector4f> boneWeights;
    std::vector<GLuint>  indices;
    std::map<int,matrix> offsets; //for the bones, applied before everything
                                  //else, in addition to interpolated
                                  //transformations assume it's the same for
                                  //each mesh for now, if we do more than 1 mesh
    //-- would be part of seperate meshes
    bone rootBone;

    matrix centerAndResize;

    graphicsEntity(std::vector<vector4f> position,std::vector<vector3f> normals,
                   std::vector<vector4f> colors  ,std::vector<vector4i> boneIDs,
                   std::vector<vector4f> bWeights,std::vector<GLuint>   indices,
                   std::map<int,matrix>, bone, matrix);

    graphicsEntity(rawAttribPtr_t<vector4f> pos, rawAttribPtr_t<vector3f> norms,
                   rawAttribPtr_t<vector4f> cols,rawAttribPtr_t<vector4i> bIDs,
                   rawAttribPtr_t<vector4f> bWts,std::vector<GLuint> indices,
                   std::map<int,matrix>, bone, matrix);

    typedef vertexAttrib::attribsList_t attribsList_t;
    attribsList_t getAttribList(std::string const&) const;
};

} //end namespace gx
#endif //GRAPHICSENTITY_H
