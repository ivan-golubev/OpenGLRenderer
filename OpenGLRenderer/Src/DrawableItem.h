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

        void Draw(glm::mat4& MVP_Matrix);

        bool WireframeMode{ false };
    private:
        std::shared_ptr<ShaderProgram> Shader;
        GLuint MVP_Matrix_Location{0};
        GLuint ViewMatrixLocation{ 0 };
        GLuint ProjectionMatrixLocation{0};
        GLuint TextureId{0};

        GLuint VertexBufferObject{0};
        GLuint VertexColorBufferObject{0};
        GLuint ElementArrayBufferObject{0}; /* indices */
        GLuint TextureCoordBuffer{0}; /* uv-coordinates */
        GLuint VertexArrayObject{0};
        GLenum DrawMode;
        GLuint NumIndices;   
    };
}
