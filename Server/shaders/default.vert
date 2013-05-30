uniform  mat4 modelToWorld;
 
in vec4  position;
in vec4  color;
in vec2  diffuseCoord;
in vec3  normal;
in float normDiff;

 
out vec4  interpColor;
out vec2  interpDiffuseCoord;
out float interpNormDiff;
out vec3  interpNormal;
out vec4  interpPosition; //position of vert / frag in eye space

out vec3  light1Dir; //direction of light in eye space
 
void main() {
  light1Dir = vec3(viewMatrix * (light1position - modelToWorld * position));

  //the correct way
  //mat3 normalMatrix = inverse(transpose(mat3(viewMatrix * modelToWorld)));
  //works as long as we don't scale models non-uniformly
  mat3 normalMatrix = mat3(viewMatrix * modelToWorld);

  interpColor    =                                          color;
  interpDiffuseCoord =                                      diffuseCoord;
  interpNormDiff =                                          normDiff;
  interpNormal   =                           normalMatrix * normal;
  interpPosition =              viewMatrix * modelToWorld * position;
  gl_Position    = projMatrix * viewMatrix * modelToWorld * position;
}
