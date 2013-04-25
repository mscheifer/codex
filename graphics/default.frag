in vec4 Color;
in vec3 Normal;
out vec4 outputF;
 
void main() {
  outputF = Color + vec4(Normal,0);
}
