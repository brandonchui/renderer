#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 transform;
uniform mat4 perspective;

void main() 
{
    
    gl_Position = perspective * transform * vec4(position, 1.0);
}
