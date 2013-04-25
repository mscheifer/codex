layout(std140) uniform display {
  mat4 viewMatrix;
  mat4 projMatrix;
};

uniform  mat4 modelToWorld;
 
in vec4 position;
in vec4 color;
in vec3 normal;
 
out vec4 Color;
out vec3 Normal;
 
void main() {
    Color = color;
    Normal = normal;
    gl_Position = projMatrix * viewMatrix * modelToWorld * position;
}
