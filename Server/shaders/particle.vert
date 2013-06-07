in vec4 position;
in vec2 texCoord;
in vec4 color;

out vec2 interpTexCoord;
out vec4 interpColor;

void main()
{
  interpTexCoord = texCoord;
  interpColor= color;
  gl_Position = projMatrix * viewMatrix * position;
}