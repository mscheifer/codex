#ifndef MESH_H
#define	MESH_H
#include <map>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include "vector3.h"
#include "vector4.h"
#include "drawSet.h"
#include "texture.h"

namespace gx {

class Mesh {
  #define INVALID_MATERIAL 0xFFFFFFFF
  public:
    Mesh(const std::string& Filename);
    Mesh(const Mesh&);// = delete; //don't copy
    Mesh& operator=(const Mesh&);// = delete; //don't assign
    Mesh(Mesh&&);
    Mesh& operator=(Mesh&&);// = delete; //define later
    ~Mesh();

    struct MeshEntry {
        MeshEntry(const aiMesh*);
        MeshEntry(const MeshEntry&);// = delete; //don't copy
        MeshEntry& operator=(const MeshEntry&);// = delete; //don't assign
        MeshEntry(MeshEntry&&) noexcept;
        MeshEntry& operator=(MeshEntry&&);// = delete; //define later

		    drawSet::vaoData_t entitiesData;

        unsigned int MaterialIndex;
    };

	  struct BoundParam {
		  vector3f center;	// center coord of model
		  float width;	// width (along x axis)
		  float height;	// height (along y axis)
		  float depth;	// width (along z axis)
	  } m_boundary;

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;

    const aiScene* m_scene;
  private:
    const aiScene* LoadMesh(const std::string& Filename);

    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

	  // fill in our m_boundary object with the boundary info
	  void CalcBoundBox(const aiScene* scene);
};

Mesh loadMeshFromFile(const std::string&);

} //end namespace gx
#endif	/* MESH_H */
