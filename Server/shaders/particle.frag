in vec2 interpTexCoord;
in vec4 interpColor;

out vec4 outputF;

uniform sampler2D diffuseTex;

void main()
{
  vec4 color = interpColor * texture(diffuseTex, interpTexCoord);
  color.w = color.w * color.x; 
  outputF = color;
}