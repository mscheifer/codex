#include "graphicsEntity.h"

namespace gx {
std::vector<gx::staticEntity> loadCube() {
  std::array<GLfloat,8*4> posArray    = {{ 0.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           0.0f, 1.0f, 0.0f, 1.0f,
                                           1.0f, 1.0f, 0.0f, 1.0f,
                                           0.0f, 0.0f,-1.0f, 1.0f,
                                           1.0f, 0.0f,-1.0f, 1.0f,
                                           0.0f, 1.0f,-1.0f, 1.0f,
                                           1.0f, 1.0f,-1.0f, 1.0f }};
  std::array<GLfloat,8*4> blueArray   = {{ 0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f }};
  std::array<GLfloat,8*4> redArray =    {{ 1.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f }};
  std::array<GLfloat,8*3> normalArray = {{-1.0f,-1.0f, 1.0f,
                                           1.0f,-1.0f, 1.0f, 
                                          -1.0f, 1.0f, 1.0f, 
                                           1.0f, 1.0f, 1.0f, 
                                          -1.0f,-1.0f,-1.0f, 
                                           1.0f,-1.0f,-1.0f, 
                                          -1.0f, 1.0f,-1.0f, 
                                           1.0f, 1.0f,-1.0f }};
  std::array<GLfloat,8> normDiffArray;
  for(size_t i = 0; i < normalArray.size(); i += 3) {
    gx::vector3f norm(normalArray[i],normalArray[i+1],normalArray[i+2]);
    norm.normalize();
    //normal diff calculatio is just a hack that works for this cube
    normDiffArray[i/3] =((norm - gx::vector3f(normalArray[i],  0,0)).magnitude()
                       + (norm - gx::vector3f(0,normalArray[i+1],0)).magnitude()
                       + (norm - gx::vector3f(0,0,normalArray[i+2])).magnitude())
                       / 3;
    normalArray[i]   = norm.x;
    normalArray[i+1] = norm.y;
    normalArray[i+2] = norm.z;
  }
  std::array<GLuint,6*6>  indicesArray = {{ 0, 1, 2, 1, 3, 2,
                                            2, 3, 6, 3, 7, 6,
                                            4, 0, 6, 0, 2, 6,
                                            1, 5, 3, 5, 7, 3,
                                            4, 5, 0, 5, 1, 0,
                                            5, 4, 7, 4, 6, 7 }};

  std::vector<GLfloat> positions(      posArray.begin(),       posArray.end());
  std::vector<GLfloat> blue(          blueArray.begin(),      blueArray.end());
  std::vector<GLfloat> red(            redArray.begin(),       redArray.end());
  std::vector<GLfloat> normals(     normalArray.begin(),    normalArray.end());
  std::vector<GLfloat> normDiffs( normDiffArray.begin(),  normDiffArray.end());
  std::vector<GLuint>  indices(    indicesArray.begin(),   indicesArray.end());

  auto positionsAttrib = std::make_shared<gx::vertexAttrib>("position",4,0,positions);
  auto      blueAttrib = std::make_shared<gx::vertexAttrib>("color"   ,4,0,blue);
  auto       redAttrib = std::make_shared<gx::vertexAttrib>("color"   ,4,0,red);
  auto   normalsAttrib = std::make_shared<gx::vertexAttrib>("normal"  ,3,0,normals);
  auto  normDiffAttrib = std::make_shared<gx::vertexAttrib>("normDiff",1,0,normDiffs);

  std::vector<std::shared_ptr<const gx::vertexAttrib>> blueCubeAttribs;
  blueCubeAttribs.push_back(positionsAttrib);
  blueCubeAttribs.push_back(blueAttrib);
  blueCubeAttribs.push_back(normalsAttrib);
  blueCubeAttribs.push_back(normDiffAttrib);

  std::vector<std::shared_ptr<const gx::vertexAttrib>> redCubeAttribs;
  redCubeAttribs.push_back(positionsAttrib);
  redCubeAttribs.push_back(redAttrib);
  redCubeAttribs.push_back(normalsAttrib);
  redCubeAttribs.push_back(normDiffAttrib);

  std::vector<staticEntity> ret;
  ret.push_back(staticEntity(indices,blueCubeAttribs));
  ret.push_back(staticEntity(indices,redCubeAttribs));

  return ret;
}

gx::staticEntity loadSkybox() {
  //sky box
  std::array<GLfloat,8*4> skyboxVtArr = {{ 1000.0f, 1000.0f, 500.0f, 1.0f,
	                                        -1000.0f, 1000.0f, 500.0f, 1.0f,
                                          -1000.0f,-1000.0f, 500.0f, 1.0f,
                                           1000.0f,-1000.0f, 500.0f, 1.0f,
                                           1000.0f, 1000.0f,   0.0f, 1.0f,
	                                        -1000.0f, 1000.0f,   0.0f, 1.0f,
                                          -1000.0f,-1000.0f,   0.0f, 1.0f,
                                           1000.0f,-1000.0f,   0.0f, 1.0f }};
  std::array<GLfloat,8*3> skyboxNormA = {{-1.0f,-1.0f,-1.0f,
                                           1.0f,-1.0f,-1.0f, 
                                           1.0f, 1.0f,-1.0f, 
                                          -1.0f, 1.0f,-1.0f, 
                                          -1.0f,-1.0f, 0.0f, 
                                           1.0f,-1.0f, 0.0f, 
                                           1.0f, 1.0f, 0.0f, 
                                          -1.0f, 1.0f, 0.0f }};
  std::array<GLfloat,8*4> lightblueArray   = {{ 0.4f, 0.4f, 1.0f, 1.0f,
                                                0.4f, 0.4f, 1.0f, 1.0f,
                                                0.4f, 0.4f, 1.0f, 1.0f,
                                                0.4f, 0.4f, 1.0f, 1.0f,
                                                0.4f, 0.4f, 1.0f, 1.0f,
                                                0.4f, 0.4f, 1.0f, 1.0f,
                                                0.4f, 0.4f, 1.0f, 1.0f,
                                                0.4f, 0.4f, 1.0f, 1.0f }};
  for(size_t i = 0; i < skyboxNormA.size(); i += 3) {
    gx::vector3f norm(skyboxNormA[i],skyboxNormA[i+1],skyboxNormA[i+2]);
    norm.normalize();
    skyboxNormA[i]   = norm.x;
    skyboxNormA[i+1] = norm.y;
    skyboxNormA[i+2] = norm.z;
  }
  std::array<GLuint,5*6> skyboxIndices = {{ 0, 2, 1, 0, 3, 2,
                                            1, 5, 0, 0, 5, 4,
                                            0, 7, 3, 0, 4, 7,
                                            2, 6, 1, 6, 5, 1,
                                            3, 6, 2, 3, 7, 6}};
  
  std::vector<GLfloat> skyboxVerts(    skyboxVtArr.begin(),   skyboxVtArr.end());
  std::vector<GLfloat> skyboxnormals(  skyboxNormA.begin(),   skyboxNormA.end());
  std::vector<GLuint>  skyboxindices(skyboxIndices.begin(), skyboxIndices.end());
  std::vector<GLfloat> lightblue(   lightblueArray.begin(),lightblueArray.end());
  auto     skyboxPosAttrib = std::make_shared<gx::vertexAttrib>("position",4,0,skyboxVerts);
  auto   skyboxColorAttrib = std::make_shared<gx::vertexAttrib>("color"   ,4,0,lightblue);
  auto skyboxNormalsAttrib = std::make_shared<gx::vertexAttrib>("normal"  ,3,0,skyboxnormals);

  std::vector<std::shared_ptr<const gx::vertexAttrib>> skyboxAttribs;
  skyboxAttribs.push_back(skyboxPosAttrib);
  skyboxAttribs.push_back(skyboxColorAttrib);
  skyboxAttribs.push_back(skyboxNormalsAttrib);

  return staticEntity(skyboxindices,skyboxAttribs);
}
} //end namespace gx
