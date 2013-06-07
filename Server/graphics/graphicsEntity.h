#ifndef GRAPHICSENTITY_H
#define GRAPHICSENTITY_H
#include <string>
#include <vector>
#include <map>
#include "bone.h"
#include "material.h"
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"
#include "vertexAttrib.h"

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
rawAttrib<T> makeRawAttrib(std::string name, std::vector<T> data);

template<typename T>
std::shared_ptr<rawAttrib<T>> makeRawAttribPtr(std::string, std::vector<T>);

struct graphicsEntity {
    template<typename T>
    struct rawAttribPtr_t {
      typedef std::shared_ptr<rawAttrib<T>> t;
    };
    //-- this block
    rawAttribPtr_t<vector4f>::t positions;
    rawAttribPtr_t<vector3f>::t normals;
    rawAttribPtr_t<vector2f>::t diffuseCoords;
    //rawAttribPtr_t<vector4f>::t colors;
    rawAttribPtr_t<vector4i>::t boneIDs;
    rawAttribPtr_t<vector4f>::t boneWeights;
    std::vector<GLuint>  indices;
    std::map<int,matrix> offsets; //for the bones, applied before everything
                                  //else, in addition to interpolated
                                  //transformations assume it's the same for
                                  //each mesh for now, if we do more than 1 mesh
    //unsigned int materialIndex;
    //-- would be part of seperate meshes
    material mat; //would be a vector if we had more than one mesh    

    std::vector<double> animationDurations;
    bone rootBone;

    matrix centerAndResize;

    graphicsEntity(std::vector<vector4f> position,std::vector<vector3f> normals,
                   std::vector<vector2f> diffTxCo,std::vector<vector4i> boneIDs,
                   std::vector<vector4f> bWeights,std::vector<GLuint>   indices,
                   std::map<int,matrix>, material, std::vector<double>, bone, matrix);

    graphicsEntity(
      rawAttribPtr_t<vector4f>::t positions,  rawAttribPtr_t<vector3f>::t norms,
      rawAttribPtr_t<vector2f>::t diffuseCoos,rawAttribPtr_t<vector4i>::t bIDs,
      rawAttribPtr_t<vector4f>::t bWts,       std::vector<GLuint> indices,
      std::map<int,matrix>, material, std::vector<double>, bone, matrix);

    graphicsEntity(graphicsEntity const&);// = delete; //don't copy
    graphicsEntity& operator=(graphicsEntity const&);// = delete; //don't copy
    graphicsEntity(graphicsEntity&&) noexcept;// = delete; //don't copy
    graphicsEntity& operator=(graphicsEntity&&);// = delete; //don't copy

    typedef vertexAttrib::attribsList_t attribsList_t;
    attribsList_t getAttribList(std::string const&) const;
};

} //end namespace gx
#endif //GRAPHICSENTITY_H
