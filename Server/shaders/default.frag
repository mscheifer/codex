in vec4  interpColor;
in vec2	 interpDiffuseCoord;
in float interpNormDiff;
in vec3  interpNormal;
in vec4  interpPosition; //position of frag in eye space

in vec3  light1Dir; //direction of light in world space

out vec4 outputF;

uniform sampler2D diffuseTex;

const float outlineThickness = 0.2f;
 
void main() {
  //because the position is in eye space, it's the same as the view direction
  vec3 viewDirection = normalize(-vec3(interpPosition));
  vec3 normal = normalize(interpNormal);

  float light1Dist = length(light1Dir);

  float att = 1.0 / (constantAttenuation + linearAttenuation * light1Dist +
                     quadraticAttenuation * light1Dist * light1Dist);
  
  vec4 color = vec4(1,0.2,0,1) /*texture(diffuseTex, interpDiffuseCoord)*/ +
               att * light1color * max(dot(normal,normalize(light1Dir)),0);

  if(dot(viewDirection, normal) < outlineThickness) {
    color = vec4(0.0,0.0,0.0,1.0);
  }
  if(interpNormDiff > 3) {
    //color = vec4(0.0,0.0,0.0,1.0); //disable until better models
  }
  outputF = color;
}
