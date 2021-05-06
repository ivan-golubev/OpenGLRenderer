#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 VertColor;

out vec4 Color;

uniform mat4 Transform;

void main()
{
    gl_Position = Transform * vec4(Position, 1.0f);
    Color = VertColor;
}
