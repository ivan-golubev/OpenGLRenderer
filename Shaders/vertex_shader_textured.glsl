#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 VertColor;
layout (location = 2) in vec2 VertTexCoord;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0f);
    Color = VertColor;
    TexCoord = VertTexCoord;
}
