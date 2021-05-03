#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 VertColor;

out vec4 Color;

void main()
{
    gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);
    Color = VertColor;
}
