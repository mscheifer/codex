in vec2	 interpDiffuseCoord;
in float interpNormDiff;
in vec3  interpNormal;
in vec4  interpPosition; //position of frag in eye space

const int maxLights = 10; //must be the same as defined in lights.h

in vec3  lightDir[maxLights]; //direction of light in world space

out vec4 outputF;

uniform sampler2D diffuseTex;

const float outlineThickness = 0.2f;

void main() {
  //because the position is in eye space, it's the same as the view direction
  vec3 viewDirection = normalize(-vec3(interpPosition));
  vec3 normal = normalize(interpNormal);

  const vec4 ambient = vec4(0.4, 0.4, 0.4, 1.0);

  vec4 lightVal = ambient;
  for(uint i = 0u; i < numLights; i++) {
    float lightDist = length(lightDir[i]);

	float att = 1.0 / (constantAttenuation + linearAttenuation * lightDist +
                     quadraticAttenuation * lightDist * lightDist);

	lightVal += att * lightColor * max(dot(normal,normalize(lightDir[i])),0);
  }

  vec4 color = texture(diffuseTex, interpDiffuseCoord) * lightVal;
  color = vec4(color.xyz * color.w,1.0);

  //if(dot(viewDirection, normal) < outlineThickness) {
  //  color = vec4(0.0,0.0,0.0,1.0);
  //}
  //if(interpNormDiff > 3) {
    //color = vec4(0.0,0.0,0.0,1.0); //disable until better models
  //}
  outputF = color;
}
