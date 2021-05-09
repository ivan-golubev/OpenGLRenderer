#pragma once

#include <glm/glm.hpp>
#include <memory>

using GLuint = unsigned int;
using GLenum = unsigned int;

namespace awesome
{
    struct Mesh;
    class ShaderProgram;
 
    class DrawableItem
    {
    public:
        DrawableItem(Mesh& mesh, std::shared_ptr<ShaderProgram> s);
        ~DrawableItem();

        DrawableItem(DrawableItem&& other) noexcept;

        void Draw(glm::mat4& transform);

        bool WireframeMode{ false };
    private:
        std::shared_ptr<ShaderProgram> Shader;
        GLuint TransformLocation;
        GLuint TextureId;

        GLuint VertexBufferObject;
        GLuint VertexColorBufferObject;
        GLuint ElementArrayBufferObject; /* indices */
        GLuint TextureCoordBuffer; /* uv-coordinates */
        GLuint VertexArrayObject;
        GLenum DrawMode;
        GLuint NumIndices;        
    };
}
