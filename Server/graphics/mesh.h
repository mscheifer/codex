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
 
enum VertComponentType {
	POSITION, 
	TEXTURE, 
	NORMAL, 
	COLOR
}; 

struct Vertex
{
  vector4 m_pos;
  vector3 m_tex;
  vector3 m_normal;
  vector4 m_color;

  Vertex() {}

  Vertex(vector4 pos, vector3 tex, vector3 normal, vector4 color)
       : m_pos(pos), m_tex(tex), m_normal(normal), m_color(color) {}
};


class Mesh
{
public:
    Mesh();

    ~Mesh();

    const aiScene* LoadMesh(const std::string& Filename);

    void Render();

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

		    void Init(std::vector<Vertex>& Vertices, 
				    const std::vector<unsigned int>& Indices);

		    drawSet::vaoData_t entitiesData;

        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;


private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

};

} //end namespace gx
#endif	/* MESH_H */
