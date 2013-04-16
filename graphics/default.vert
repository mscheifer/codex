layout(std140) uniform display {
  mat4 viewMatrix;
  mat4 projMatrix;
};
 
in vec4 position;
in vec4 color;
 
out vec4 Color;
 
void main() {
    Color = color;
    gl_Position = projMatrix * viewMatrix * position;
}
