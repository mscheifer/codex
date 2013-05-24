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

void printNodes(aiNode* node, int level) {
  if(node == nullptr) return;
  for(int i = 0; i < level; i++) {
    gx::debugout << "  ";
  }
  gx::debugout << node->mName.C_Str() << " meshes ";
  gx::debugout << node->mNumMeshes << gx::endl;
  for(int i = 0; i < level; i++) {
    gx::debugout << "  ";
  }
  gx::debugout << gx::toMat(node->mTransformation) << gx::endl;
  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    printNodes(node->mChildren[i],level+1);
  }
}

std::vector<GLuint> initIndices(const aiMesh* paiMesh) {
  std::vector<GLuint> Indices;

  for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
      const aiFace& Face = paiMesh->mFaces[i];
      assert(Face.mNumIndices == 3);
      Indices.push_back(Face.mIndices[0]);
      Indices.push_back(Face.mIndices[1]);
      Indices.push_back(Face.mIndices[2]);
  }
  return Indices;
}

gx::Mesh::attribsList_t initAttribs(const gx::Mesh::idMap_t& idMap,
    const aiMesh* paiMesh) {
  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  std::vector<GLfloat> positions;
  std::vector<GLfloat> colors;
  std::vector<GLfloat> normals;
  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
    const aiVector3D* pPos = &(paiMesh->mVertices[i]);
    const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
    const aiVector3D* pTexCoord =
      paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
    gx::vector4f posVec = /*resize **/ toVec4(*pPos);
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
    gx::vector3f normVec = /*resize **/ toVec3(*pNormal);
    normals.push_back(normVec.x);
	  normals.push_back(normVec.y);
	  normals.push_back(normVec.z);
    //TODO: use texture coordinates
	}
  const std::array<std::pair<GLfloat,GLint>,gx::Mesh::maxBonesPerVertex>
	                           defaultBones = {{std::make_pair(1.0f,-1),
                                              std::make_pair(0.0f,-1),
                                              std::make_pair(0.0f,-1),
                                              std::make_pair(0.0f,-1)}};
  std::vector<std::array<std::pair<GLfloat,GLint>,gx::Mesh::maxBonesPerVertex>>
    boneGroups(paiMesh->mNumVertices,defaultBones);
  for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {
    const auto& paiBone = paiMesh->mBones[i];
    for(unsigned int j = 0; j < paiBone->mNumWeights; j++) {
      const auto& paiWeight = paiBone->mWeights[j];
      auto& bg = boneGroups[paiWeight.mVertexId];
      bool good = false;
	    for(auto bgitr = bg.begin(); bgitr != bg.end(); bgitr++) {
		    auto& b = *bgitr;
        if(b.second == -1) {
          b.first = paiWeight.mWeight;
          b.second = idMap.find(paiBone->mName.C_Str())->second;
          good = true;
          break;
        }
      }
      if(!good) std::cout << "Error too many bones for one vertex" << std::endl;
    }
  }
  std::vector<GLfloat> boneWeights;
  std::vector<GLint>   boneIds;
  for(auto bgitr = boneGroups.begin(); bgitr != boneGroups.end(); bgitr++) {
    const auto& b = *bgitr;
    float totalWeight = 0;
    for(auto bitr = b.begin(); bitr != b.end(); bitr++) {
      const auto& a = *bitr;
      boneWeights.push_back(a.first);
      totalWeight += a.first;
      boneIds.push_back(a.second);
    }
    if(totalWeight - 1.0 >= 0.01 || totalWeight - 1.0 <= -0.01) {
      std::cout << "error, total weight: " << totalWeight << std::endl;
    }
  }

	auto positionsAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("position"   ,4,0,positions));
	auto colorsAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("color"      ,4,0,colors));
  auto normalsAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("normal"     ,3,0,normals));
  auto boneWeightAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("boneWeights",4,0,boneWeights));
  auto boneIdAttrib = std::make_shared<gx::vertexAttrib>(
    gx::vertexAttrib("boneIDs"    ,4,0,boneIds));

  gx::Mesh::attribsList_t attribs;
	attribs.push_back(positionsAttrib);
	attribs.push_back(colorsAttrib);
	attribs.push_back(normalsAttrib);
	attribs.push_back(boneWeightAttrib);
	attribs.push_back(boneIdAttrib);

	return attribs;
}

gx::bone makeBone(gx::Mesh::idMap_t& idMap, int& nextId,
                  const std::map<std::string,gx::matrix>&                      offsets,
                  const std::map<std::string,std::vector<const aiNodeAnim*>>&  animations,
                  const aiNode* bon) {
  gx::matrix transform = gx::toMat(bon->mTransformation);
  gx::matrix offset = gx::identity;
  bool real = false;
  int id = -1;
  if(offsets.find(bon->mName.C_Str()) != offsets.end()) {
    offset = offsets.find(bon->mName.C_Str())->second;
    real = true;
    id = nextId++;
  }
  idMap.insert(std::make_pair(bon->mName.C_Str(),id));
  std::vector<std::vector<gx::bone::key>> boneAnimations;
  const auto& anims = animations.find(bon->mName.C_Str())->second;
  for(auto itr = anims.begin(); itr != anims.end(); ++itr) {
    const aiNodeAnim* animation = *itr;
    std::vector<gx::bone::key> animKeys;
    if(animation != nullptr) {
      for(unsigned int i = 0; i < animation->mNumPositionKeys; i++) {
        gx::bone::key k;
        k.position = animation->mPositionKeys[i];
        k.rotation = animation->mRotationKeys[i];
        k.scaling  = animation->mScalingKeys [i];
        animKeys.push_back(k);
      }
    }
    boneAnimations.push_back(std::move(animKeys));
  }
  std::vector<gx::bone> children;
  for(unsigned int i = 0; i < bon->mNumChildren; i++) {
    children.push_back(makeBone(idMap,nextId,offsets,animations,bon->mChildren[i]));
  }
  return gx::bone(id,std::move(offset),std::move(transform),real,
                  std::move(boneAnimations),std::move(children));
}

//should create an iterator for the node tree
std::map<std::string,std::vector<const aiNodeAnim*>> walkNodesForAnims(const aiNode* node,unsigned int n){
  std::map<std::string,std::vector<const aiNodeAnim*>> ret;
  ret.insert(std::make_pair(node->mName.C_Str(),
                            std::vector<const aiNodeAnim*>(n,nullptr)));
  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    const auto& ch = walkNodesForAnims(node->mChildren[i],n);
    ret.insert(ch.begin(),ch.end());
  }
  return ret;
}

gx::bone initBones(std::map<std::string,unsigned int>& idMap,
    const aiScene* scene, const gx::matrix resize) {
  std::map<std::string,gx::matrix>                     offsets;
  std::map<std::string,std::vector<const aiNodeAnim*>> animations;
  //just do the first mesh for now
  animations = walkNodesForAnims(scene->mRootNode,scene->mNumAnimations);
  for(unsigned int i = 0; i < scene->mMeshes[0]->mNumBones; i++) {
    const auto& meshBone = scene->mMeshes[0]->mBones[i];
    offsets.insert(
      std::make_pair(meshBone->mName.C_Str(),gx::toMat(meshBone->mOffsetMatrix)));
  }
  for(unsigned int i = 0; i < scene->mNumAnimations; i++) {
    const aiAnimation* anim = scene->mAnimations[i];
    for(unsigned int j = 0; j < anim->mNumChannels; j++) {
      const aiNodeAnim* nodeAnim = anim->mChannels[j];
      animations.at(nodeAnim->mNodeName.C_Str()).at(i) = nodeAnim;
      //TODO: fix for different numbers of keys
      if(nodeAnim->mNumPositionKeys != nodeAnim->mNumRotationKeys) {
        std::cout << "Error " << nodeAnim->mNumPositionKeys << " != ";
        std::cout << nodeAnim->mNumRotationKeys << std::endl;
      }
      if(nodeAnim->mNumPositionKeys != nodeAnim->mNumScalingKeys) {
        std::cout << "Error " << nodeAnim->mNumPositionKeys << " != ";
        std::cout << nodeAnim->mNumScalingKeys << std::endl;
      }
    }
  }
  int nextIds = 0;
  auto boneTree = makeBone(idMap,nextIds,std::move(offsets),std::move(animations),
                           scene->mRootNode);
  boneTree.transform = resize * boneTree.transform;
  return boneTree;
}
} //end unnamed namespace

gx::Mesh::MeshEntry::MeshEntry(idMap_t& ids, const aiMesh* paiMesh)
  : attribs(initAttribs(ids,paiMesh)), indices(initIndices(paiMesh)),
    MaterialIndex(paiMesh->mMaterialIndex)                                 {
  debugout << "mesh num bones: " << paiMesh->mNumBones << endl;
  for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {
    debugout << "  bone: " << paiMesh->mBones[i]->mName.C_Str();
    debugout << " name_length: " << paiMesh->mBones[i]->mName.length;
    debugout << " numVerts: " << paiMesh->mBones[i]->mNumWeights << endl;
    debugout << "  offset: " << toMat(paiMesh->mBones[i]->mOffsetMatrix) << endl;
  }
};

gx::Mesh::MeshEntry::MeshEntry(MeshEntry&& other) noexcept
  : attribs(std::move(other.attribs)), indices(std::move(other.indices)),
    MaterialIndex(std::move(other.MaterialIndex)) {}

gx::Mesh::MeshEntry& gx::Mesh::MeshEntry::operator=(MeshEntry&& other){
  *((int*) nullptr) = 1;
  return *this;
}

gx::Mesh::Mesh(const std::string& Filename, length_t height)
	: mImporter(), mScene(LoadFile(mImporter, Filename)),
    m_boundary(CalcBoundBox(mScene, height)), idMap(),
    bones(initBones(idMap,mScene, m_boundary.centerAndResize)),
    m_Entries(InitFromScene(idMap,mScene)),
    m_Textures(InitMaterials(mScene, Filename)),
	//just do the first one unless kangh has a model with more
    entityData(m_Entries[0].indices,m_Entries[0].attribs,std::move(bones)) {}

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
  debugout << "num animations: " << pScene->mNumAnimations   << endl;
  debugout << "num meshes: "     << pScene->mNumMeshes       << endl;
  debugout << "num textures: "   << pScene->mNumTextures     << endl;
  debugout << "nodes: "          << endl;
  printNodes(pScene->mRootNode,0);
  for(unsigned int i = 0; i < pScene->mNumAnimations; i++) {
    debugout << "animation: " << pScene->mAnimations[i]->mName.C_Str() << endl;
    debugout << "duration: " << pScene->mAnimations[i]->mDuration << endl;
    debugout << "tics per second: " << pScene->mAnimations[i]->mTicksPerSecond << endl;
    debugout << "meshes: " << pScene->mAnimations[i]->mNumMeshChannels << endl;
    debugout << "bones: " << pScene->mAnimations[i]->mNumChannels << endl;
    for(unsigned int j = 0; j < pScene->mAnimations[i]->mNumChannels; j++) {
      debugout << "  bonesName: " << pScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str() << endl;
      debugout << "    position keys " << pScene->mAnimations[i]->mChannels[j]->mNumPositionKeys << endl;
      debugout << "    rotation keys " << pScene->mAnimations[i]->mChannels[j]->mNumRotationKeys << endl;
      debugout << "    scaling keys " << pScene->mAnimations[i]->mChannels[j]->mNumScalingKeys << endl;
    }
  }
  //end print
  return pScene;
}

std::vector<gx::Mesh::MeshEntry> gx::Mesh::InitFromScene(idMap_t& idMap,
     const aiScene* pScene) {
  if(pScene == nullptr) return std::vector<MeshEntry>();

  if(pScene->mNumMeshes > 1) {
    std::cout << "Warning: more than 1 mesh in model" << std::endl;
  }

  std::vector<MeshEntry> Ret;

  // Initialize the meshes in the scene one by one
  for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++) {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    Ret.push_back(MeshEntry(idMap,paiMesh));
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
  debugout << "scale val: " << modelHeight / Ret.height << endl;
  Ret.centerAndResize =
    uniformScaling(modelHeight / Ret.height) *
    translation(-center.x,-center.y,-center.z);

  return Ret;
}
