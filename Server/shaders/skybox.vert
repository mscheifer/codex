in vec4 position;

out vec3 TexCoord0;

const mat4 modelToWorld = mat4(1, 0, 0, 0,
                               0, 1, 0, 0,
							   0, 0, 1, 0,
							   0, 0, 1000, 1);

void main()
{
    gl_Position = projMatrix * viewMatrix * modelToWorld * position;
    TexCoord0 = position.xyz;
}