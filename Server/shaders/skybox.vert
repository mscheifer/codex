in vec4 position;

out vec3 TexCoord0;

void main()
{
    gl_Position = projMatrix * viewMatrix * position;
    TexCoord0 = position.xyz;
}
