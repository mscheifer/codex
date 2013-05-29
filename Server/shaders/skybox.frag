in vec3 TexCoord0;

out vec4 outputF;

uniform samplerCube gCubemapTexture;

void main()
{
    outputF = texture(gCubemapTexture, TexCoord0);
}
