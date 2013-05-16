#include "mesh.h"
#include <assimp/Importer.hpp>    // C++ importer interface
#include <assimp/scene.h>			    // Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <assert.h>
#include <string>
#include <algorithm>
#include <limits>
#include "matrix.h"
#include "vertexAttrib.h"

namespace {
gx::vector4f toVec4(const aiVector3D& aiVec) {
  return gx::vector4f(aiVec.x,aiVec.y,aiVec.z);
}
gx::vector3f toVec3(const aiVector3D& aiVec) {
  return gx::vector3f(aiVec.x,aiVec.y,aiVec.z);
}

void printNodes(aiNode* node, int level) {
  if(node == nullptr) return;
  for(int i = 0; i < level; i++) {
    std::cout << "  ";
  }
  std::cout << node->mName.C_Str() << std::endl;
  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    printNodes(node->mChildren[i],level+1);
  }
}

gx::drawSet::vaoData_t initEntitiesData(const aiMesh* paiMesh, const gx::matrix resize) {
  std::vector<GLuint> Indices;

  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
      const aiFace& Face = paiMesh->mFaces[i];
      assert(Face.mNumIndices == 3);
      Indices.push_back(Face.mIndices[0]);
      Indices.push_back(Face.mIndices[1]);
      Indices.push_back(Face.mIndices[2]);
  }

  std::vector<GLfloat> positions;
  std::vector<GLfloat> colors;
  std::vector<GLfloat> normals;
  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
    const aiVector3D* pPos = &(paiMesh->mVertices[i]);
    const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
    const aiVector3D* pTexCoord =
      paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
    gx::vector4f posVec = resize * toVec4(*pPos);
	  positions.push_back(posVec.x);
	  positions.push_back(posVec.y);
	  positions.push_back(posVec.z);
	  positions.push_back(posVec.w);
      // temporary color filler. 
			// colors are stored in materials
	  colors.push_back(1.0f);
	  colors.push_back(0.2f);
	  colors.push_back(0.0f);
	  colors.push_back(1.0f);
    //resize works for normals too because we're doing uniform scaling
    gx::vector3f normVec = resize * toVec3(*pNormal);
    normals.push_back(normVec.x);
	  normals.push_back(normVec.y);
	  normals.push_back(normVec.z);
    //TODO: use texture coordinates
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

	return std::make_pair(Indices,attribs);
}
} //end unnamed namespace

gx::Mesh::MeshEntry::MeshEntry(const aiMesh* paiMesh, const matrix resize)
  : entitiesData(initEntitiesData(paiMesh,resize)),
    MaterialIndex(paiMesh->mMaterialIndex)                                 {
  std::cout << "mesh num bones: " << paiMesh->mNumBones << std::endl;
  for(int i = 0; i < paiMesh->mNumBones; i++) {
    std::cout << "  bone: " << paiMesh->mBones[i]->mName.C_Str() << std::endl;
  }
};

gx::Mesh::MeshEntry::MeshEntry(MeshEntry&& other) noexcept
  : entitiesData (std::move(other.entitiesData)),
    MaterialIndex(std::move(other.MaterialIndex)) {}

gx::Mesh::Mesh(const std::string& Filename, length_t height)
	: m_Entries(), m_Textures(), m_Good(LoadMesh(Filename, height)) {
}

bool gx::Mesh::LoadMesh(const std::string& Filename, length_t height) 
{    
  bool Ret = false;
  Assimp::Importer Importer;

  const aiScene* pScene = Importer.ReadFile(Filename.c_str(), 
	      aiProcess_Triangulate	     |
	      aiProcess_GenSmoothNormals |
	      aiProcess_FlipUVs);
  std::cout << "num animations: " << pScene->mNumAnimations   << std::endl;
  std::cout << "num meshes: "     << pScene->mNumMeshes       << std::endl;
  std::cout << "num textures: "   << pScene->mNumTextures     << std::endl;
  std::cout << "nodes: "          << std::endl;
  printNodes(pScene->mRootNode,0);
  for(unsigned int i = 0; i < pScene->mNumAnimations; i++) {
    std::cout << "animation: " << pScene->mAnimations[i]->mName.C_Str() << std::endl;
    std::cout << "duration: " << pScene->mAnimations[i]->mDuration << std::endl;
    std::cout << "tics per second: " << pScene->mAnimations[i]->mTicksPerSecond << std::endl;
    std::cout << "meshes: " << pScene->mAnimations[i]->mNumMeshChannels << std::endl;
    std::cout << "bones: " << pScene->mAnimations[i]->mNumChannels << std::endl;
  }
  if (pScene) {
      Ret = InitFromScene(pScene, Filename, height);
  } else {
      std::cout << "Error parsing '" <<  Filename.c_str() << "': '";
      std::cout << Importer.GetErrorString() << std::endl;
  }
  return Ret;
}

bool gx::Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename,
                             const length_t height) {
  // get bounding box
  CalcBoundBox(pScene,height);

  if(pScene->mNumMeshes > 1) {
    std::cout << "Warning: more than 1 mesh in model" << std::endl;
  }

  // Initialize the meshes in the scene one by one
  for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++) {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    this->m_Entries.push_back(
      MeshEntry(paiMesh,this->m_boundary.centerAndResize));
  }

  return InitMaterials(pScene, Filename);
}

bool gx::Mesh::InitMaterials(const aiScene* pScene,const std::string& Filename){
  // Extract the directory part from the file name
  std::string::size_type SlashIndex = Filename.find_last_of("/");
  std::string Dir;

  if (SlashIndex == std::string::npos) {
    Dir = ".";
  } else if (SlashIndex == 0) {
    Dir = "/";
  } else {
    Dir = Filename.substr(0, SlashIndex);
  }

  bool Ret = true;

  // Initialize the materials
  for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
    const aiMaterial* pMaterial = pScene->mMaterials[i];

    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString Path;

      if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS) {
        std::string FullPath = Dir + "/" + Path.data;
        this->m_Textures.push_back(Texture(GL_TEXTURE_2D, FullPath.c_str()));

        if (!m_Textures.back().Load()) {
          std::cout<<"Error loading texture '" << FullPath.c_str() << std::endl;
          this->m_Textures.pop_back();
          Ret = false;
        } else {
          std::cout << "Loaded texture '" << FullPath.c_str() << std::endl; 
        }
      }
    } 
    // Load a white texture in case the model does not include its own texture
    if (!Ret) {
      this->m_Textures.push_back(Texture(GL_TEXTURE_2D, "./white.png"));

      Ret = this->m_Textures.back().Load();
    }
  }
  return Ret;
}

void gx::Mesh::CalcBoundBox(const aiScene* scene, length_t modelHeight) {
	// using aiMatrix4x4 instead of gx::matrix since there's more operations
  // defined for it.
	// for now, we prolly don't need to use it. Read the comment below for
	// the aiTransformVecByMatrix4 function for further explanation
	//aiMatrix4x4* transform = &(scene->mRootNode->mTransformation);

	gx::vector3f minVec;
	gx::vector3f maxVec;

	// only calculate the first mesh because our models only have one
	const aiMesh* mesh = scene->mMeshes[0];

	// transform the vertex based on the node's transformation matrix.
	// we prolly don't have to do this for now since currently we don't
	// apply the transformation on our model before drawing.
	//aiTransformVecByMatrix4(&foo, transform);

  const aiVector3D* vertexStart = mesh->mVertices;
  const aiVector3D* vertexEnd   = mesh->mVertices + mesh->mNumVertices;
  auto xCompare= [](const aiVector3D& a,const aiVector3D& b){return a.x < b.x;};
  auto yCompare= [](const aiVector3D& a,const aiVector3D& b){return a.y < b.y;};
  auto zCompare= [](const aiVector3D& a,const aiVector3D& b){return a.z < b.z;};

  minVec.x = std::min_element(vertexStart,vertexEnd,xCompare)->x;
  minVec.y = std::min_element(vertexStart,vertexEnd,yCompare)->y;
  minVec.z = std::min_element(vertexStart,vertexEnd,zCompare)->z;

  maxVec.x = std::max_element(vertexStart,vertexEnd,xCompare)->x;
  maxVec.y = std::max_element(vertexStart,vertexEnd,yCompare)->y;
  maxVec.z = std::max_element(vertexStart,vertexEnd,zCompare)->z;
	// calc boundary center pos & length info
	this->m_boundary.center.set(
		(minVec.x + maxVec.x) / 2.0f,		// x position
		(minVec.y + maxVec.y) / 2.0f,		// y position
		(minVec.z + maxVec.z) / 2.0f);	// z position

	this->m_boundary.width  = maxVec.x - minVec.x;
	this->m_boundary.height = maxVec.y - minVec.y;
	this->m_boundary.depth  = maxVec.z - minVec.z;

  auto center = this->m_boundary.center;
  std::cout << "scale val: " << modelHeight / this->m_boundary.height << std::endl;
  this->m_boundary.centerAndResize =
    uniformScaling(modelHeight / this->m_boundary.height) *
    translation(-center.x,-center.y,-center.z);
}
