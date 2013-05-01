layout(std140) uniform display {
  mat4 viewMatrix;
  mat4 projMatrix;
  vec3 cameraPos; //camera position in world space
};

layout(std140) uniform light1 {
  vec4  light1Position;
  vec4  light1Color;
  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;
};

uniform  mat4 modelToWorld;
 
in vec4  position;
in vec4  color;
in vec3  normal;
in float normDiff;
 
out vec4  interpColor;
out float interpNormDiff;
out vec3  interpNormal;
out vec4  interpPosition; //position of vert / frag in eye space

out vec3  light1Dir; //direction of light in eye space
 
void main() {
  light1Dir = vec3(viewMatrix * (light1Position - modelToWorld * position));

  //the correct way
  //mat3 normalMatrix = inverse(transpose(mat3(viewMatrix * modelToWorld)));
  //works as long as we don't scale models non-uniformly
  mat3 normalMatrix = mat3(viewMatrix * modelToWorld);

  interpColor    =                                          color;
  interpNormDiff =                                          normDiff;
  interpNormal   =                           normalMatrix * normal;
  interpPosition =  modelToWorld * position;
  gl_Position    =  modelToWorld * position;
}