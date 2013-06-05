in vec4 position;
in vec2 texCoord;
in vec4 color;

out vec2 interpTexCoord;
out vec4 interpColor;

void main()
{
  interpTexCoord = texCoord;
  interpColor= color;
  mat4 billboard;
  billboard[0] = vec4(1,0,0,viewMatrix[0][3]);
  billboard[1] = vec4(0,0,1,viewMatrix[1][3]);
  billboard[2] = vec4(0,1,0,viewMatrix[2][3]);
  billboard[3] = viewMatrix[3];
  //gl_Position = projMatrix * billboard * position;
  gl_Position = projMatrix * viewMatrix * position;
}