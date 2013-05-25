#include "mesh.h"
#include <assert.h>

namespace {
gx::vector4f toVec4(const aiVector3D& aiVec) {
  return gx::vector4f(aiVec.x,aiVec.y,aiVec.z);
}
gx::vector3f toVec3(const aiVector3D& aiVec) {
  return gx::vector3f(aiVec.x,aiVec.y,aiVec.z);
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
          b.second =
             static_cast<GLint>(idMap.find(paiBone->mName.C_Str())->second);
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
} //end unnamed namespace

gx::Mesh::MeshEntry::MeshEntry(idMap_t& ids, const aiMesh* paiMesh)
  : attribs(initAttribs(ids,paiMesh)), indices(initIndices(paiMesh)),
    MaterialIndex(paiMesh->mMaterialIndex)                                 {
  debugout << "mesh num bones: " << paiMesh->mNumBones << endl;
  for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {
    debugout << "  bone: " << paiMesh->mBones[i]->mName.C_Str();
    debugout << " name_length: " << paiMesh->mBones[i]->mName.length;
    debugout << " numVerts: " << paiMesh->mBones[i]->mNumWeights << endl;
    debugout << "  offset: " << toMat(paiMesh->mBones[i]->mOffsetMatrix)<< endl;
  }
};

gx::Mesh::MeshEntry::MeshEntry(MeshEntry&& other) noexcept
  : attribs(std::move(other.attribs)), indices(std::move(other.indices)),
    MaterialIndex(std::move(other.MaterialIndex)) {}

gx::Mesh::MeshEntry& gx::Mesh::MeshEntry::operator=(MeshEntry&&) {
  exit(-1); //fail hard
  return *this;
}
