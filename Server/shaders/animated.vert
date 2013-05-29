const int MAX_BONES = 30;

uniform  mat4 modelToWorld;
uniform  mat4 boneTransformations[MAX_BONES];
 
in vec4  position;
in vec4  color;
in vec2  diffuseCoord;
in vec3  normal;
in float normDiff;
in ivec4 boneIDs;
in vec4  boneWeights;
 
out vec4  interpColor;
out vec2  interpDiffuseCoord;
out float interpNormDiff;
out vec3  interpNormal;
out vec4  interpPosition; //position of vert / frag in eye space

out vec3  light1Dir; //direction of light in eye space
 
void main() {
  //fix this logic for no bone possibility
  mat4 boneTransform;
  boneTransform[0] = vec4(0,0,0,0);
  boneTransform[1] = vec4(0,0,0,0);
  boneTransform[2] = vec4(0,0,0,0);
  boneTransform[3] = vec4(0,0,0,0);
  if(boneIDs[0] != -1) boneTransform += boneTransformations[boneIDs[0]] * boneWeights[0];
  if(boneIDs[1] != -1) boneTransform += boneTransformations[boneIDs[1]] * boneWeights[1];
  if(boneIDs[2] != -1) boneTransform += boneTransformations[boneIDs[2]] * boneWeights[2];
  if(boneIDs[3] != -1) boneTransform += boneTransformations[boneIDs[3]] * boneWeights[3];

  light1Dir = vec3(viewMatrix * (light1position - modelToWorld * position));

  //the correct way
  //mat3 normalMatrix = inverse(transpose(mat3(viewMatrix * modelToWorld)));
  //this works as long as we don't scale models non-uniformly
  mat3 normalMatrix = mat3(viewMatrix * modelToWorld * boneTransform);

  interpColor    =                                                          color;
  interpDiffuseCoord =                                                      diffuseCoord;
  interpNormDiff =                                                          normDiff;
  interpNormal   =                                           normalMatrix * normal;
  interpPosition =              viewMatrix * modelToWorld * boneTransform * position;
  gl_Position    = projMatrix * viewMatrix * modelToWorld * boneTransform * position;
}
