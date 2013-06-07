uniform  mat4 modelToWorld;
 
in vec4  position;
in vec2  diffuseCoord;
in vec3  normal;


out vec2  interpDiffuseCoord;

out vec3  interpNormal;
out vec4  interpPosition; //position of vert / frag in eye space

const int maxLights = 15; //must be the same as defined in lights.h

out vec3  lightDir[maxLights]; //direction of light in eye space
 
void main() {
  for(uint i = 0u; i < numLights; i++) {
	lightDir[i] = vec3(viewMatrix * (lightPosition[i] - modelToWorld * position));
  }

  //the correct way
  //mat3 normalMatrix = inverse(transpose(mat3(viewMatrix * modelToWorld)));
  //works as long as we don't scale models non-uniformly
  mat3 normalMatrix = mat3(viewMatrix * modelToWorld);

  interpDiffuseCoord =                                      diffuseCoord;
  interpNormal   =                           normalMatrix * normal;
  interpPosition =              viewMatrix * modelToWorld * position;
  gl_Position    = projMatrix * viewMatrix * modelToWorld * position;
}
