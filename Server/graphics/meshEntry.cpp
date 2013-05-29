#include "mesh.h"
#include <assert.h>
#include "assimpUtil.h"

namespace {
std::vector<gx::vector4f> initPositions(const aiMesh* paiMesh) {
  std::vector<gx::vector4f> ret;
  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
    const aiVector3D& pPos = paiMesh->mVertices[i];
    ret.push_back(gx::toVec4(pPos));
  }
  return ret;
}

std::vector<gx::vector3f> initNormals(const aiMesh* paiMesh) {
  std::vector<gx::vector3f> ret;
  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
    const aiVector3D& pNormal = paiMesh->mNormals[i];
    ret.push_back(gx::toVec3(pNormal));
  }
  return ret;
}

std::pair<std::vector<gx::vector4i>,std::vector<gx::vector4f>>
initBoneWeights(const gx::Mesh::idMap_t& idMap, const aiMesh* paiMesh) {
  const std::pair<gx::vector4i,gx::vector4f> defaultBones =
    std::make_pair(gx::vector4i(-1,-1,-1,-1),gx::vector4f(1.0f,0.0f,0.0f,0.0f));
  std::pair<std::vector<gx::vector4i>,std::vector<gx::vector4f>> ret;
  std::vector<std::pair<gx::vector4i,gx::vector4f>>
    boneGroups(paiMesh->mNumVertices,defaultBones);
  for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {
    const auto& paiBone = paiMesh->mBones[i];
    for(unsigned int j = 0; j < paiBone->mNumWeights; j++) {
      const auto& paiWeight = paiBone->mWeights[j];
      auto& bg = boneGroups[paiWeight.mVertexId];
      bool good = false;
      auto wItr = bg.second.begin();
      for(auto idItr = bg.first.begin();
          idItr != bg.first.end() && wItr != bg.second.end(); idItr++, wItr++) {
        auto& id = *idItr;
        if(id == -1) {
          id    = static_cast<GLint>(idMap.find(paiBone->mName.C_Str())->second);
          *wItr = paiWeight.mWeight;
          good = true;
          break;
        }
      }
      if(!good) std::cout << "Error too many bones for one vertex" << std::endl;
    }
  }
  for(auto bgitr = boneGroups.begin(); bgitr != boneGroups.end(); bgitr++) {
    const auto& b = *bgitr;
    float totalWeight = 0;
    for(auto bitr = b.second.begin(); bitr != b.second.end(); bitr++) {
      const auto& a = *bitr;
      totalWeight += a;
    }
    if(totalWeight - 1.0 >= 0.01 || totalWeight - 1.0 <= -0.01) {
      std::cout << "error, total weight: " << totalWeight << std::endl;
    }
    ret.first.push_back(b.first);
    ret.second.push_back(b.second);
  }
  return ret;
}

std::vector<gx::vector2f> initTexCoords(const aiMesh* paiMesh) {
  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  std::vector<gx::vector2f> ret;
  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
    //TODO: fix for more materials
    const aiVector3D* pTexCoord =
      paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
    ret.push_back(gx::vector2f(pTexCoord->x,pTexCoord->y));
  }

  return ret;
}

std::vector<gx::vector4f> initColors(const aiMesh* paiMesh) {
  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  std::vector<gx::vector4f> ret;
  for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
      // temporary color filler. 
      // colors are stored in materials
    ret.push_back(gx::vector4f(1.0f,0.2f,0.0f));
  }

  return ret;
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

std::map<int,gx::matrix> initOffsets(const gx::Mesh::idMap_t& ids, const aiMesh* paiMesh) {
  std::map<int,gx::matrix> offsets;
  for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {
    const auto& meshBone = paiMesh->mBones[i];
    if(ids.find(meshBone->mName.C_Str()) == ids.end()) {
      std::cout << "error, no id for " << meshBone->mName.C_Str() << std::endl;
    }
    offsets.insert(std::make_pair(
      ids.find(meshBone->mName.C_Str())->second,gx::toMat(meshBone->mOffsetMatrix)));
  }
  return offsets;
}
} //end unnamed namespace

gx::Mesh::MeshEntry::MeshEntry(const idMap_t& ids, const aiMesh* paiMesh)
  : positions(initPositions(paiMesh)), diffuseCoords(initTexCoords(paiMesh)),
    normals(initNormals(paiMesh)), boneWeights(initBoneWeights(ids,paiMesh)),
    indices(initIndices(paiMesh)), offsets(initOffsets(ids,paiMesh)),
    MaterialIndex(paiMesh->mMaterialIndex) {
  debugout << "mesh num colors: " << paiMesh->GetNumColorChannels() << endl;
  debugout << "mesh num bones: " << paiMesh->mNumBones << endl;
  for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {
    debugout << "  bone: " << paiMesh->mBones[i]->mName.C_Str();
    debugout << " name_length: " << paiMesh->mBones[i]->mName.length;
    debugout << " numVerts: " << paiMesh->mBones[i]->mNumWeights << endl;
    //debugout<< "  offset: "<< toMat(paiMesh->mBones[i]->mOffsetMatrix)<< endl;
  }
};

gx::Mesh::MeshEntry::MeshEntry(MeshEntry&& other) noexcept
  : positions(std::move(other.positions)), diffuseCoords(std::move(other.diffuseCoords)),
    normals(std::move(other.normals)),boneWeights(std::move(other.boneWeights)),
    indices(std::move(other.indices)),    offsets(std::move(other.offsets)),
    MaterialIndex(std::move(other.MaterialIndex)) {}

gx::Mesh::MeshEntry& gx::Mesh::MeshEntry::operator=(MeshEntry&&) {
  exit(-1); //fail hard
  return *this;
}
