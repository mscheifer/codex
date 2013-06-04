in vec2 interpTexCoord;

out vec4 outputF;

uniform sampler2D diffuseTex;

void main()
{
  vec4 color = texture(diffuseTex, interpTexCoord);
  outputF = /*vec4(1.0, 0.0, 0.0, 1.0) **/ color;
}