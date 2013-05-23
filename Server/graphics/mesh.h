#ifndef MESH_H
#define	MESH_H
#include <GL/glew.h>
#include <vector>
#include <assimp/Importer.hpp>    // C++ importer interface
#include "matrix.h"
#include "texture.h"
#include "graphicsEntity.h"

struct aiMesh;

namespace gx {
class Mesh {
  public:
    static const unsigned int maxBonesPerVertex = 4;

    typedef staticEntity::attribsList_t attribsList_t;
    typedef float length_t;

    Mesh(const std::string& Filename,length_t);
    Mesh(const Mesh&);// = delete; //don't copy
    Mesh& operator=(const Mesh&);// = delete; //don't assign
    Mesh(Mesh&&);// = delete //define later
    Mesh& operator=(Mesh&&);// = delete; //define later

    typedef std::map<std::string,unsigned int> idMap_t;

    struct MeshEntry {
        MeshEntry(idMap_t&,const aiMesh*,const matrix);
        MeshEntry(const MeshEntry&);// = delete; //don't copy
        MeshEntry& operator=(const MeshEntry&);// = delete; //don't assign
        MeshEntry(MeshEntry&&) noexcept;
        MeshEntry& operator=(MeshEntry&&);// = delete; //define later

               attribsList_t attribs;
         std::vector<GLuint> indices;   
                unsigned int MaterialIndex;
    };

    //this needs to be a member so the aiScene stays in scope
    Assimp::Importer mImporter;
      const aiScene* mScene;

	  struct BoundParam {
      matrix centerAndResize;
		  vector3f center;	// center coord of model
		  length_t width;	  // width  (along x axis)
		  length_t depth;	  // width  (along y axis)
		  length_t height;	// height (along z axis)
	  } m_boundary;

    idMap_t idMap;
       bone bones;

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture>   m_Textures;
             dynamicEntity entityData;

    bool                   m_Good();
  private:
    static const aiScene*        LoadFile(Assimp::Importer&,const std::string&);
    static std::vector<MeshEntry> InitFromScene(idMap_t&, const aiScene*, const matrix);
    static std::vector<Texture>   InitMaterials(const aiScene*,
                                                const std::string& Filename);
	  // fill in our m_boundary object with the boundary info
	  static BoundParam CalcBoundBox(const aiScene* scene,length_t);
};
} //end namespace gx
#endif	/* MESH_H */
