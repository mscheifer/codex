#ifndef MESH_H
#define  MESH_H
#include <GL/glew.h>
#include <vector>
#include <set>
#include <assimp/Importer.hpp>    // C++ importer interface
#include "graphicsEntity.h"
#include "material.h"
#include "matrix.h"
#include "texture.h"

struct aiMesh;

namespace gx {
class Mesh {
  public:
    static const unsigned int maxBonesPerVertex = 4;
    typedef float length_t;

    Mesh(const std::string& Filename,length_t);
    Mesh(const Mesh&);// = delete; //don't copy
    Mesh& operator=(const Mesh&);// = delete; //don't assign
    Mesh(Mesh&&);// = delete //define later
    Mesh& operator=(Mesh&&);// = delete; //define later

    typedef std::map<std::string,unsigned int> idMap_t;

    struct MeshEntry {
        MeshEntry(idMap_t const&,const aiMesh*);
        MeshEntry(const MeshEntry&);// = delete; //don't copy
        MeshEntry& operator=(const MeshEntry&);// = delete; //don't assign
        MeshEntry(MeshEntry&&) noexcept;
        MeshEntry& operator=(MeshEntry&&);// = delete; //define later

        std::vector<vector4f> positions;
        std::vector<vector2f> diffuseCoords;
        std::vector<vector3f> normals;
        std::pair<std::vector<vector4i>,
                  std::vector<vector4f>> boneWeights;
          std::vector<GLuint> indices;   

         std::map<int,matrix> offsets; //for the bones
                 unsigned int MaterialIndex;
    };

    //this needs to be a member so the aiScene stays in scope
    Assimp::Importer mImporter;
      const aiScene* mScene;

    struct BoundParam {
      matrix centerAndResize;
      vector3f center;  // center coord of model
      length_t width;    // width  (along x axis)
      length_t depth;    // width  (along y axis)
      length_t height;  // height (along z axis)
    } m_boundary;

    idMap_t idMap;
       bone bones;

    std::vector<MeshEntry> m_Entries;
    std::vector<material>  m_Materials;
            graphicsEntity entityData;

    bool                   m_Good();
  private:
    static const aiScene*        LoadFile(Assimp::Importer&,const std::string&);
    static std::vector<MeshEntry> InitFromScene(idMap_t&, const aiScene*);
    static std::vector<material>  InitMaterials(const aiScene*,
                                                const std::string& Filename);
    // fill in our m_boundary object with the boundary info
    static BoundParam CalcBoundBox(const aiScene* scene,length_t);
};
} //end namespace gx
#endif  /* MESH_H */
