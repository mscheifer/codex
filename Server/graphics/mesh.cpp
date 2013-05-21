#include "mesh.h"
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

gx::matrix toMat(aiMatrix4x4 m) {
  return gx::matrix(m.a1, m.a2, m.a3, m.a4,
                    m.b1, m.b2, m.b3, m.b4,
                    m.c1, m.c2, m.c3, m.c4,
                    m.d1, m.d2, m.d3, m.d4);
}

void printNodes(aiNode* node, int level) {
  if(node == nullptr) return;
  for(int i = 0; i < level; i++) {
    std::cout << "  ";
  }
  std::cout << node->mName.C_Str() << " meshes ";
  std::cout << node->mNumMeshes << std::endl;
  for(int i = 0; i < level; i++) {
    std::cout << "  ";
  }
  std::cout << toMat(node->mTransformation) << std::endl;
  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    printNodes(node->mChildren[i],level+1);
  }
}

gx::dynamicEntity initEntitiesData(const aiMesh* paiMesh, const gx::matrix resize) {
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

	auto positionsAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("position",4,0,positions));
	auto colorsAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("color",4,0,colors));
  auto normalsAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("normal",3,0,normals));

	std::vector<std::shared_ptr<const gx::vertexAttrib>> attribs;
	attribs.push_back(positionsAttrib);
	attribs.push_back(colorsAttrib);
	attribs.push_back(normalsAttrib);

	return {Indices,attribs};
}
} //end unnamed namespace

gx::Mesh::MeshEntry::MeshEntry(const aiMesh* paiMesh, const matrix resize)
  : entitiesData(initEntitiesData(paiMesh,resize)),
    MaterialIndex(paiMesh->mMaterialIndex)                                 {
  std::cout << "mesh num bones: " << paiMesh->mNumBones << std::endl;
  for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {
    std::cout << "  bone: " << paiMesh->mBones[i]->mName.C_Str();
    std::cout << " name_length: " << paiMesh->mBones[i]->mName.length;
    std::cout << " numVerts: " << paiMesh->mBones[i]->mNumWeights << std::endl;
    std::cout << "  offset: " << toMat(paiMesh->mBones[i]->mOffsetMatrix) << std::endl;
  }
};

gx::Mesh::MeshEntry::MeshEntry(MeshEntry&& other) noexcept
  : entitiesData (std::move(other.entitiesData)),
    MaterialIndex(std::move(other.MaterialIndex)) {}

gx::Mesh::Mesh(const std::string& Filename, length_t height)
	: mImporter(), mScene(LoadFile(mImporter, Filename)),
    m_boundary(CalcBoundBox(mScene, height)),
    m_Entries(InitFromScene(mScene, m_boundary.centerAndResize)),
    m_Textures(InitMaterials(mScene, Filename)) {}

const aiScene* gx::Mesh::LoadFile(Assimp::Importer& Importer,const std::string& Filename) {
  const aiScene* pScene = Importer.ReadFile(Filename.c_str(), 
	      aiProcess_Triangulate	     |
	      aiProcess_GenSmoothNormals |
	      aiProcess_FlipUVs);
  if (!pScene) {
    std::cout << "Error parsing '" <<  Filename.c_str() << "': '";
    std::cout << Importer.GetErrorString() << std::endl;
  }
  //print
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
    for(unsigned int j = 0; j < pScene->mAnimations[i]->mNumChannels; j++) {
      std::cout << "  bonesName: " << pScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str() << std::endl;
      std::cout << "    position keys " << pScene->mAnimations[i]->mChannels[j]->mNumPositionKeys << std::endl;
      std::cout << "    rotation keys " << pScene->mAnimations[i]->mChannels[j]->mNumRotationKeys << std::endl;
      std::cout << "    scaling keys " << pScene->mAnimations[i]->mChannels[j]->mNumScalingKeys << std::endl;
    }
  }
  //end print
  return pScene;
}

std::vector<gx::Mesh::MeshEntry> gx::Mesh::InitFromScene(const aiScene* pScene,
                                                         const matrix centerAndResize) {
  if(pScene == nullptr) return std::vector<MeshEntry>();

  if(pScene->mNumMeshes > 1) {
    std::cout << "Warning: more than 1 mesh in model" << std::endl;
  }

  std::vector<MeshEntry> Ret;

  // Initialize the meshes in the scene one by one
  for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++) {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    Ret.push_back(MeshEntry(paiMesh,centerAndResize));
  }

  return Ret;
}

std::vector<gx::Texture> gx::Mesh::InitMaterials(const aiScene* pScene,const std::string& Filename){
  if(pScene == nullptr) return std::vector<Texture>();
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

  std::vector<Texture> Ret;

  // Initialize the materials
  for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
    const aiMaterial* pMaterial = pScene->mMaterials[i];

    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString Path;

      if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS) {
        std::string FullPath = Dir + "/" + Path.data;
        Ret.push_back(Texture(GL_TEXTURE_2D, FullPath.c_str()));

        if (!Ret.back().Load()) {
          std::cout<<"Error loading texture '" << FullPath.c_str() << std::endl;
          Ret.pop_back();
          // Load a white texture in case the model does not include its own texture
          Ret.push_back(Texture(GL_TEXTURE_2D, "./white.png"));
          Ret.back().Load();
        } else {
          std::cout << "Loaded texture '" << FullPath.c_str() << std::endl; 
        }
      }
    } 
  }
  return Ret;
}

gx::Mesh::BoundParam gx::Mesh::CalcBoundBox(const aiScene* scene, length_t modelHeight) {
	// using aiMatrix4x4 instead of gx::matrix since there's more operations
  // defined for it.
	// for now, we prolly don't need to use it. Read the comment below for
	// the aiTransformVecByMatrix4 function for further explanation
	//aiMatrix4x4* transform = &(scene->mRootNode->mTransformation);

	gx::vector3f minVec;
	gx::vector3f maxVec;

  BoundParam Ret;

  if(scene == nullptr) return Ret;

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
	Ret.center.set(
		(minVec.x + maxVec.x) / 2.0f,		// x position
		(minVec.y + maxVec.y) / 2.0f,		// y position
		(minVec.z + maxVec.z) / 2.0f);	// z position

	Ret.width  = maxVec.x - minVec.x;
	Ret.depth  = maxVec.y - minVec.y;
	Ret.height = maxVec.z - minVec.z;

  auto center = Ret.center;
  std::cout << "scale val: " << modelHeight / Ret.height << std::endl;
  Ret.centerAndResize =
    uniformScaling(modelHeight / Ret.height) *
    translation(-center.x,-center.y,-center.z);

  return Ret;
}
