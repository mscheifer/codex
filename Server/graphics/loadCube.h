#include "drawSet.h"

namespace gx {
std::vector<gx::drawSet::vaoData_t> loadCube() {
  std::array<GLfloat,8*4> posArray    = {{ 0.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           0.0f, 1.0f, 0.0f, 1.0f,
                                           1.0f, 1.0f, 0.0f, 1.0f,
                                           0.0f, 0.0f,-1.0f, 1.0f,
                                           1.0f, 0.0f,-1.0f, 1.0f,
                                           0.0f, 1.0f,-1.0f, 1.0f,
                                           1.0f, 1.0f,-1.0f, 1.0f }};
  std::array<GLfloat,8*4>   blueArray = {{ 0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f }};
  std::array<GLfloat,8*4> redArray =    {{ 0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f }};
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
    gx::vector3 norm(normalArray[i],normalArray[i+1],normalArray[i+2]);
    norm.normalize();
    //normal diff calculatio is just a hack that works for this cube
    normDiffArray[i/3] =((norm - gx::vector3(normalArray[i],  0,0)).magnitude()
                       + (norm - gx::vector3(0,normalArray[i+1],0)).magnitude()
                       + (norm - gx::vector3(0,0,normalArray[i+2])).magnitude())
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

  gx::vertexAttrib* positionsAttrib =
    new gx::vertexAttrib("position",4,0,positions);
  gx::vertexAttrib* blueAttrib =
    new gx::vertexAttrib("color"   ,4,0,blue);
  gx::vertexAttrib* redAttrib =
    new gx::vertexAttrib("color"   ,4,0,red);
  gx::vertexAttrib* normalsAttrib =
    new gx::vertexAttrib("normal"  ,3,0,normals);
  gx::vertexAttrib* normDiffAttrib =
    new gx::vertexAttrib("normDiff",1,0,normDiffs);

  std::vector<const gx::vertexAttrib*> blueCubeAttribs;
  blueCubeAttribs.push_back(positionsAttrib);
  blueCubeAttribs.push_back(blueAttrib);
  blueCubeAttribs.push_back(normalsAttrib);
  blueCubeAttribs.push_back(normDiffAttrib);

  std::vector<const gx::vertexAttrib*> redCubeAttribs;
  redCubeAttribs.push_back(positionsAttrib);
  redCubeAttribs.push_back(redAttrib);
  redCubeAttribs.push_back(normalsAttrib);
  redCubeAttribs.push_back(normDiffAttrib);

  std::vector<drawSet::vaoData_t> ret;
  ret.push_back(std::make_pair(indices,blueCubeAttribs));
  ret.push_back(std::make_pair(indices,redCubeAttribs));

  return ret;
}
} //end namespace gx
