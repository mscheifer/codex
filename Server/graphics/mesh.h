#ifndef MESH_H
#define	MESH_H
#include <GL/glew.h>
#include <assimp/Importer.hpp>    // C++ importer interface
#include <assimp/scene.h>			    // Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <vector>
#include "drawSet.h"
#include "texture.h"

namespace gx {
class Mesh {
  public:
    typedef float length_t;
    Mesh(const std::string& Filename,length_t);
    Mesh(const Mesh&);// = delete; //don't copy
    Mesh& operator=(const Mesh&);// = delete; //don't assign
    Mesh(Mesh&&);// = delete //define later
    Mesh& operator=(Mesh&&);// = delete; //define later

    struct MeshEntry {
        MeshEntry(const aiMesh*,const matrix);
        MeshEntry(const MeshEntry&);// = delete; //don't copy
        MeshEntry& operator=(const MeshEntry&);// = delete; //don't assign
        MeshEntry(MeshEntry&&) noexcept;
        MeshEntry& operator=(MeshEntry&&);// = delete; //define later

		    drawSet::vaoData_t entitiesData;

        unsigned int MaterialIndex;
    };

	  struct BoundParam {
      matrix centerAndResize;
		  vector3f center;	// center coord of model
		  length_t width;	// width  (along x axis)
		  length_t height;	// height (along y axis)
		  length_t depth;	// width  (along z axis)
	  } m_boundary;

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture> m_Textures;
    bool m_Good;
  private:
    bool LoadMesh(const std::string& Filename,length_t);

    bool InitFromScene(const aiScene*, const std::string& Filename,length_t);
    bool InitMaterials(const aiScene*, const std::string& Filename);

	  // fill in our m_boundary object with the boundary info
	  void CalcBoundBox(const aiScene* scene,length_t);
};

Mesh loadMeshFromFile(const std::string&);

} //end namespace gx
#endif	/* MESH_H */
