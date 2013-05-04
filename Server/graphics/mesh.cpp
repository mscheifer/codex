#include <assert.h>
#include <GL/glew.h>
#include <string>

#include "mesh.h"
#include "vertexAttrib.h"
#include "vector4.h"

gx::Mesh::MeshEntry::MeshEntry()
{
    //VB = INVALID_OGL_VALUE;
    //IB = INVALID_OGL_VALUE;
    NumIndices  = 0;
    MaterialIndex = INVALID_MATERIAL;
};

gx::Mesh::MeshEntry::~MeshEntry()
{
	/*
    if (VB != INVALID_OGL_VALUE)
    {
        //glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE)
    {
        //glDeleteBuffers(1, &IB);
    } */
}

std::vector<GLfloat> vertToComponentV(std::vector<gx::Vertex>& Vertices,
									                    int sizePerVertex) {
	
	std::vector<GLfloat> component(Vertices.size()*sizePerVertex);

	std::vector<gx::Vertex>::iterator v;
	for (v = Vertices.begin(); v != Vertices.end(); ++v) {
		// push in x,y,z,w component of the vertex
		for (int i = 0; i < sizePerVertex ; i++) {
			component.push_back(v->m_pos[i]);
		}
	}
	return component;
}

void gx::Mesh::MeshEntry::Init(std::vector<Vertex>& Vertices,
						   const std::vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();

	gx::vertexAttrib* positionsAttrib = new
    gx::vertexAttrib("position",4,0,vertToComponentV(Vertices, 4));
	gx::vertexAttrib* colorsAttrib = new
    gx::vertexAttrib("color",4,0,vertToComponentV(Vertices, 4));
	gx::vertexAttrib* normalsAttrib = new
    gx::vertexAttrib("normal"  ,3,0,vertToComponentV(Vertices, 3));

	std::vector<const gx::vertexAttrib*> attribs;
	attribs.push_back(positionsAttrib);
	attribs.push_back(colorsAttrib);
	attribs.push_back(normalsAttrib);

	this->entitiesData = std::make_pair(Indices,attribs);
}

gx::Mesh::Mesh()
{
}


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
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, Filename);
}

void gx::Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
    
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
	
	// init no longer makes opengl calls anymore.
	// instead, it creates a list of vaoData_t which has all
	// the vertex attribute properties.
	m_Entries[Index].Init(Vertices, Indices);
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

