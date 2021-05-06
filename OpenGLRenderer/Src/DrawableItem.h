#pragma once

#include <glm/glm.hpp>

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

        void Draw(glm::mat4& transform);

    private:
        GLuint ShaderProgramId;
        GLuint TransformLocation;

        GLuint VertexBufferObject;
        GLuint VertexColorBufferObject;
        GLuint ElementArrayBufferObject; /* indices */
        GLuint VertexArrayObject;
        GLenum DrawMode;
        GLuint NumIndices;
    };
}
