// shaders/fs.glsl
#version 410 core
uniform vec4 triangleColor;    // Add this uniform declaration
out vec4 color;
void main() {
    color = triangleColor;     // Use the uniform instead of hardcoded color
}
