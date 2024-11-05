// shaders/fs.glsl
#version 410 core

uniform vec4 triangleColor;
out vec4 color;

void main() {
    color = triangleColor;
}
