#pragma once

using GLuint = unsigned int;
using GLenum = unsigned int;

namespace awesome
{
    struct Mesh;
 
    class DrawableItem
    {
    public:
        DrawableItem(Mesh& mesh, GLuint shaderProgramId);
        ~DrawableItem();

        DrawableItem(DrawableItem&& other) noexcept;

        void Draw();

    private:
        GLuint ShaderProgramId;
        GLuint VertexBufferObject;
        GLuint VertexArrayObject;
        GLenum DrawMode;
    };
}
