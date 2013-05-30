#include "loadCustom.h"
#include "vector1.h"

std::vector<gx::graphicsEntity> gx::loadCube() {
  std::array<vector4f,8> posArray    = {{ vector4f( 0.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 0.0f, 1.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 1.0f, 0.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f,-1.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f,-1.0f, 1.0f),
                                          vector4f( 0.0f, 1.0f,-1.0f, 1.0f),
                                          vector4f( 1.0f, 1.0f,-1.0f, 1.0f) }};
  std::array<vector4f,8> blueArray   = {{ vector4f( 0.0f, 0.0f, 1.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f, 1.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f, 1.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f, 1.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f, 1.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f, 1.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f, 1.0f, 1.0f),
                                          vector4f( 0.0f, 0.0f, 1.0f, 1.0f) }};
  std::array<vector4f,8> redArray    = {{ vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f),
                                          vector4f( 1.0f, 0.0f, 0.0f, 1.0f) }};
  std::array<vector3f,8> normalArray = {{ vector3f(-1.0f,-1.0f, 1.0f),
                                          vector3f( 1.0f,-1.0f, 1.0f), 
                                          vector3f(-1.0f, 1.0f, 1.0f), 
                                          vector3f( 1.0f, 1.0f, 1.0f), 
                                          vector3f(-1.0f,-1.0f,-1.0f), 
                                          vector3f( 1.0f,-1.0f,-1.0f), 
                                          vector3f(-1.0f, 1.0f,-1.0f), 
                                          vector3f( 1.0f, 1.0f,-1.0f) }};
  std::vector<vector1f> normDiffs(8);
  for(auto itr = normalArray.begin(); itr != normalArray.end(); itr++) {
    auto& norm = *itr;
    norm.normalize();
    //normal diff calculatio is just a hack that works for this cube
    normDiffs.push_back(((norm - gx::vector3f(norm[0],0,0)).magnitude()
                       + (norm - gx::vector3f(0,norm[1],0)).magnitude()
                       + (norm - gx::vector3f(0,0,norm[2])).magnitude())
                       / 3);
  }
  std::array<GLuint,6*6>  indicesArray = {{ 0, 1, 2, 1, 3, 2,
                                            2, 3, 6, 3, 7, 6,
                                            4, 0, 6, 0, 2, 6,
                                            1, 5, 3, 5, 7, 3,
                                            4, 5, 0, 5, 1, 0,
                                            5, 4, 7, 4, 6, 7 }};

  std::vector<vector4f> positions(     posArray.begin(),       posArray.end());
  std::vector<vector4f> blue(         blueArray.begin(),      blueArray.end());
  std::vector<vector4f> red(           redArray.begin(),       redArray.end());
  std::vector<vector3f> normals(    normalArray.begin(),    normalArray.end());
  std::vector<GLuint>  indices(    indicesArray.begin(),   indicesArray.end());

  std::vector<vector2f> texCoords(8,vector2f(0,0));

  auto positionsAttrib = makeRawAttribPtr("position"    ,positions);
  auto  texCoordAttrib = makeRawAttribPtr("diffuseCoord",texCoords);
  auto   normalsAttrib = makeRawAttribPtr("normal"      ,normals);
  auto  normDiffAttrib = makeRawAttribPtr("normDiff"    ,normDiffs);

  auto boneIdAttrib =
    makeRawAttribPtr("boneID",std::vector<vector4i>(1,vector4i(0,-1,-1,-1)));
  auto boneWeightAttrib =
    makeRawAttribPtr("boneWeight",std::vector<vector4f>(1,vector4f(1,0,0,0)));

  bone root1(1,identity,true,std::vector<bone::animation>(),
                            std::vector<bone>());
  bone root2(1,identity,true,std::vector<bone::animation>(),
                            std::vector<bone>());

  material blueMat(Texture(GL_TEXTURE_2D, "models/white.png"),vector4f(0,0,1,1));
  material  redMat(Texture(GL_TEXTURE_2D, "models/white.png"),vector4f(1,0,0,1));

  graphicsEntity blueCube(positionsAttrib,normalsAttrib,texCoordAttrib,boneIdAttrib,
     boneWeightAttrib,indices,std::map<int,matrix>(),std::move(blueMat),
     std::move(root1),identity);
  graphicsEntity  redCube(positionsAttrib,normalsAttrib,texCoordAttrib,boneIdAttrib,
     boneWeightAttrib,indices,std::map<int,matrix>(),std::move(redMat),
     std::move(root2),identity);

  std::vector<graphicsEntity> ret;
  ret.push_back(std::move(blueCube));
  ret.push_back(std::move (redCube));

  return ret;
}

gx::graphicsEntity gx::loadSkybox() {
  //sky box
  std::array<vector4f,8> skyboxVtArr = {{ vector4f( 1000.0f, 1000.0f, 500.0f, 1.0f),
                                          vector4f(-1000.0f, 1000.0f, 500.0f, 1.0f),
                                          vector4f(-1000.0f,-1000.0f, 500.0f, 1.0f),
                                          vector4f( 1000.0f,-1000.0f, 500.0f, 1.0f),
                                          vector4f( 1000.0f, 1000.0f,   0.0f, 1.0f),
                                          vector4f(-1000.0f, 1000.0f,   0.0f, 1.0f),
                                          vector4f(-1000.0f,-1000.0f,   0.0f, 1.0f),
                                          vector4f( 1000.0f,-1000.0f,   0.0f, 1.0f) }};
  std::array<vector3f,8> skyboxNormA = {{ vector3f(-1.0f,-1.0f,-1.0f),
                                          vector3f( 1.0f,-1.0f,-1.0f), 
                                          vector3f( 1.0f, 1.0f,-1.0f), 
                                          vector3f(-1.0f, 1.0f,-1.0f), 
                                          vector3f(-1.0f,-1.0f, 0.0f), 
                                          vector3f( 1.0f,-1.0f, 0.0f), 
                                          vector3f( 1.0f, 1.0f, 0.0f), 
                                          vector3f(-1.0f, 1.0f, 0.0f) }};
  for(auto itr = skyboxNormA.begin(); itr != skyboxNormA.end(); itr++) {
    auto& norm = *itr;
    norm.normalize();
  }
  std::array<GLuint,5*6> skyboxIndices = {{ 0, 2, 1, 0, 3, 2,
                                            1, 5, 0, 0, 5, 4,
                                            0, 7, 3, 0, 4, 7,
                                            2, 6, 1, 6, 5, 1,
                                            3, 6, 2, 3, 7, 6}};
  
  std::vector<vector4f> skyboxVerts(    skyboxVtArr.begin(),   skyboxVtArr.end());
  std::vector<vector3f> skyboxnormals(  skyboxNormA.begin(),   skyboxNormA.end());
  std::vector<GLuint>  skyboxindices(skyboxIndices.begin(), skyboxIndices.end());

  std::vector<vector2f> texCoords(8,vector2f(0,0));

  std::vector<vector4i> skyboxBoneIDs(1,vector4i(0,-1,-1,-1));
  std::vector<vector4f> skyboxBWeight(1,vector4f(1, 0, 0, 0));

  std::map<int,matrix> skyboxOffsets;

  bone root(1,identity,true,std::vector<bone::animation>(),
                            std::vector<bone>());

  material mat(Texture(GL_TEXTURE_2D, "models/white.png"),vector4f(0.4f,0.4f,1.0f));

  return graphicsEntity(std::move(skyboxVerts), std::move(skyboxnormals),
    std::move(texCoords), std::move(skyboxBoneIDs), std::move(skyboxBWeight), 
    std::move(skyboxindices), skyboxOffsets, std::move(mat), std::move(root),
    identity);
}
