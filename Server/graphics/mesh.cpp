#include "mesh.h"
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include <string>
#include <algorithm>
#include <limits>
#include "assimpUtil.h"
#include "matrix.h"
#include "vertexAttrib.h"

namespace {

void printNodes(aiNode* node, int level) {
  if(node == nullptr) return;
  for(int i = 0; i < level; i++) {
    gx::debugout << "  ";
  }
  gx::debugout << node->mName.C_Str() << " meshes ";
  gx::debugout << node->mNumMeshes << gx::endl;
  //for(int i = 0; i < level; i++) {
  //  gx::debugout << "  ";
  //}
  //gx::debugout << gx::toMat(node->mTransformation) << gx::endl;
  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    printNodes(node->mChildren[i],level+1);
  }
}

gx::bone makeBone(gx::Mesh::idMap_t& idMap, int& nextId,
                                       const std::set<std::string>& realBones,
        const std::map<std::string,std::vector<const aiNodeAnim*>>& animations,
        const aiNode* bon) {
  gx::matrix transform = gx::toMat(bon->mTransformation);
  bool real = false;
  int id = -1;
  if(realBones.count(bon->mName.C_Str()) > 0) {
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
    children.push_back(makeBone(idMap,nextId,realBones,animations,
                       bon->mChildren[i]));
  }
  return gx::bone(id,std::move(transform),real,std::move(boneAnimations),
                     std::move(children));
}

//should create an iterator for the node tree
std::map<std::string,std::vector<const aiNodeAnim*>> 
    walkNodesForAnims(const aiNode* node,unsigned int n) {
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
                                        const aiScene* scene) {
  std::set<std::string>                                realBones;
  std::map<std::string,std::vector<const aiNodeAnim*>> animations;
  animations = walkNodesForAnims(scene->mRootNode,scene->mNumAnimations);
  for(unsigned int i = 0; i < scene->mNumMeshes; i++) {
    for(unsigned int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
      const auto& meshBone = scene->mMeshes[i]->mBones[j];
      realBones.insert(meshBone->mName.C_Str());
    }
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
  auto boneTree = makeBone(idMap,nextIds,std::move(realBones),
                           std::move(animations), scene->mRootNode);
  return boneTree;
}
} //end unnamed namespace

gx::Mesh::Mesh(const std::string& Filename, length_t height)
  : mImporter(), mScene(LoadFile(mImporter, Filename)),
    m_boundary(CalcBoundBox(mScene, height)), idMap(),
    bones(initBones(idMap,mScene)),
    m_Entries(InitFromScene(idMap,mScene)),
    m_Materials(InitMaterials(mScene, Filename)),
  //just do the first one unless kangh has a model with more
    entityData(std::move(m_Entries[0].positions),
      std::move(m_Entries[0].normals), std::move(m_Entries[0].diffuseCoords),
      std::move(m_Entries[0].boneWeights.first),
      std::move(m_Entries[0].boneWeights.second),
      std::move(m_Entries[0].indices), std::move(m_Entries[0].offsets),
      std::move(m_Materials[0]), std::move(bones),
      std::move(m_boundary.centerAndResize)) {}

const aiScene* gx::Mesh::LoadFile(Assimp::Importer& Importer,
                                 const std::string& Filename) {
  const aiScene* pScene = Importer.ReadFile(Filename.c_str(), 0
         | aiProcess_Triangulate
         | aiProcess_GenSmoothNormals);
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
    debugout << "tics per second: " << pScene->mAnimations[i]->mTicksPerSecond;
    debugout << endl;
    debugout << "meshes: " << pScene->mAnimations[i]->mNumMeshChannels << endl;
    debugout << "bones: " << pScene->mAnimations[i]->mNumChannels << endl;
    for(unsigned int j = 0; j < pScene->mAnimations[i]->mNumChannels; j++) {
      const auto& channel = pScene->mAnimations[i]->mChannels[j];
      debugout << "  bonesName: " << channel->mNodeName.C_Str() << endl;
      debugout << "    position keys " << channel->mNumPositionKeys << endl;
      debugout << "    rotation keys " << channel->mNumRotationKeys << endl;
      debugout << "    scaling keys " << channel->mNumScalingKeys << endl;
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

std::vector<gx::material>
gx::Mesh::InitMaterials(const aiScene* pScene,const std::string& Filename) {
  if(pScene == nullptr) return std::vector<material>();
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

  std::vector<material> ret;

  // Initialize the materials
  for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
    const aiMaterial* pMaterial = pScene->mMaterials[i];
    vector4f diffuseColor;
    aiColor3D	ColorDiffuse;
    if (AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, ColorDiffuse)) {
      diffuseColor = toVec4(ColorDiffuse);
    } else {
      diffuseColor = vector4f(1.0,1.0,1.0);
    }
    std::string diffusePath;
    aiString Path;
    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
        AI_SUCCESS == pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path)) {
      diffusePath = Dir + "/" + Path.C_Str();
    } else {
      diffusePath = "models/white.png";
    }
    Texture diffuseTex(GL_TEXTURE_2D, diffusePath);
    diffuseTex.Load();
    //material index will correspond to the Mesh's material index
    ret.push_back(material(std::move(diffuseTex),diffuseColor));
  }
  return ret;
}

gx::Mesh::BoundParam gx::Mesh::CalcBoundBox(const aiScene* scene,
                                                  length_t modelHeight) {
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
    (minVec.x + maxVec.x) / 2.0f,    // x position
    (minVec.y + maxVec.y) / 2.0f,    // y position
    (minVec.z + maxVec.z) / 2.0f);  // z position

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
