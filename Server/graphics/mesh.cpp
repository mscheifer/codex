#include <assert.h>
#include <GL/glew.h>
#include <string>

#include "mesh.h"
#include "vertexAttrib.h"
#include "vector4.h"

gx::Mesh::MeshEntry::MeshEntry(const aiMesh* paiMesh)
  : entitiesData(), MaterialIndex(paiMesh->mMaterialIndex) {
  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;

  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
      const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

      Vertex v(gx::vector4(pPos->x, pPos->y, pPos->z),
                gx::vector3(pTexCoord->x, pTexCoord->y, 0),
                gx::vector3(pNormal->x, pNormal->y, pNormal->z),
				gx::vector4(1.0f, 0.2f, 0.0f));	// temporary color filler. 
												// colors are stored in materials, not vertex

      Vertices.push_back(v);
  }

  for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
      const aiFace& Face = paiMesh->mFaces[i];
      assert(Face.mNumIndices == 3);
      Indices.push_back(Face.mIndices[0]);
      Indices.push_back(Face.mIndices[1]);
      Indices.push_back(Face.mIndices[2]);
  }

  std::vector<GLfloat> positions;
	for (auto v = Vertices.begin(); v != Vertices.end(); ++v) {
	  positions.push_back(v->m_pos.x);
	  positions.push_back(v->m_pos.y);
	  positions.push_back(v->m_pos.z);
	  positions.push_back(v->m_pos.w);
	}

  std::vector<GLfloat> colors;
	for (auto v = Vertices.begin(); v != Vertices.end(); ++v) {
	  colors.push_back(v->m_color.x);
	  colors.push_back(v->m_color.y);
	  colors.push_back(v->m_color.z);
	  colors.push_back(v->m_color.w);
	}

  std::vector<GLfloat> normals;
	for (auto v = Vertices.begin(); v != Vertices.end(); ++v) {
	  normals.push_back(v->m_normal.x);
	  normals.push_back(v->m_normal.y);
	  normals.push_back(v->m_normal.z);
	}

	gx::vertexAttrib* positionsAttrib = new
    gx::vertexAttrib("position",4,0,positions);
	gx::vertexAttrib* colorsAttrib = new
    gx::vertexAttrib("color",4,0,colors);
	gx::vertexAttrib* normalsAttrib = new
    gx::vertexAttrib("normal",3,0,normals);

	std::vector<const gx::vertexAttrib*> attribs;
	attribs.push_back(positionsAttrib);
	attribs.push_back(colorsAttrib);
	attribs.push_back(normalsAttrib);

	this->entitiesData = std::make_pair(Indices,attribs);
};

gx::Mesh::MeshEntry::MeshEntry(MeshEntry&& other)
  : entitiesData (std::move(other.entitiesData)),
    MaterialIndex(std::move(other.MaterialIndex)) {}

gx::Mesh::Mesh()
  : m_Entries(), m_Textures() {}


gx::Mesh::~Mesh()
{
    Clear();
}


void gx::Mesh::Clear()
{
    for (unsigned int i = 0 ; i < m_Textures.size() ; i++) {
        delete m_Textures[i];
    }
}


const aiScene* gx::Mesh::LoadMesh(const std::string& Filename) 
{
    // Release the previously loaded mesh (if it exists)
    Clear();
    
    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), 
		aiProcess_Triangulate	   |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs);
    
    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }

	if (Ret) {
		return pScene;
	} else {
		return 0;
	}
}

bool gx::Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{  
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        m_Entries.push_back(MeshEntry(paiMesh));
    }

    return InitMaterials(pScene, Filename);
}

bool gx::Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (!m_Textures[i]) {
            m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

            Ret = m_Textures[i]->Load();
        }
    }

    return Ret;
}

void gx::Mesh::Render()
{
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);

    //for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
    //    glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    //    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    //    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

    //    const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

    //    if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
    //        m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
    //    }

    //    glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
    //}

    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(2);
}

