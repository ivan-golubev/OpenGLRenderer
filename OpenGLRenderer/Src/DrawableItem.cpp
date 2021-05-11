#include "DrawableItem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "ModelLoader.h"
#include "ShaderProgram.h"

namespace awesome
{
	DrawableItem::DrawableItem(Mesh& mesh, std::shared_ptr<ShaderProgram> s):
        Shader{ s },
        VertexBufferObject(0),
        ElementArrayBufferObject(0),
        VertexArrayObject(0),
        DrawMode(GL_TRIANGLES),
        NumIndices(mesh.GetNumIndices())
	{
        /* get shader variable locations */
        MVP_Matrix_Location = glGetUniformLocation(Shader->shaderProgramId, "MVP");

        glGenVertexArrays(1, &VertexArrayObject);
        glGenBuffers(1, &VertexBufferObject);
        glGenBuffers(1, &VertexColorBufferObject);
        glGenBuffers(1, &ElementArrayBufferObject);
        glGenBuffers(1, &TextureCoordBuffer);
        glGenTextures(1, &TextureId);

        glBindVertexArray(VertexArrayObject);

        /* pass the vertex positions */
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);                
        glBufferData(GL_ARRAY_BUFFER, mesh.VerticesSize(), mesh.Vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, Vertex::POSITION_COMPONENTS, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        /* pass the vertex colors */
        glBindBuffer(GL_ARRAY_BUFFER, VertexColorBufferObject);
        glBufferData(GL_ARRAY_BUFFER, mesh.ColorsSize(), mesh.Colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, Color::COLOR_COMPONENTS, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);

        /* pass the indeces */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementArrayBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.IndicesSize(), mesh.Indices, GL_STATIC_DRAW);

        /* pass the texture */
        glBindTexture(GL_TEXTURE_2D, TextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh.TextureWidth, mesh.TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mesh.Texture);
        /* set the texture wrapping/filtering options */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        /* pass the uv-coordinates */
        glBindBuffer(GL_ARRAY_BUFFER, TextureCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.TextureCoordsSize(), mesh.TextureCoords, GL_STATIC_DRAW);        
        glVertexAttribPointer(2, TextureCoord::TEXTURE_COMPONENTS, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glEnableVertexAttribArray(2);

        /* unbind the VAO first, then the buffers */
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

    DrawableItem::DrawableItem(DrawableItem&& other) noexcept
    {
        Shader = other.Shader;
        TextureId = other.TextureId;
        MVP_Matrix_Location = other.MVP_Matrix_Location;
        VertexBufferObject = other.VertexBufferObject;
        ElementArrayBufferObject = other.ElementArrayBufferObject;
        VertexArrayObject = other.VertexArrayObject;
        VertexColorBufferObject = other.VertexColorBufferObject;
        TextureCoordBuffer = other.TextureCoordBuffer;
        DrawMode = other.DrawMode;
        NumIndices = other.NumIndices;

        memset(&other, 0, sizeof(DrawableItem));
    }

	DrawableItem::~DrawableItem()
	{
        if (VertexArrayObject)
            glDeleteVertexArrays(1, &VertexArrayObject);
        if (VertexBufferObject)
            glDeleteBuffers(1, &VertexBufferObject);
        if (VertexColorBufferObject)
            glDeleteBuffers(1, &VertexColorBufferObject);
        if (ElementArrayBufferObject)
            glDeleteBuffers(1, &ElementArrayBufferObject);
        if (TextureCoordBuffer)
            glDeleteBuffers(1, &TextureCoordBuffer);
    }

    void DrawableItem::Draw(glm::mat4& MVP_Matrix)
    {        
        glPolygonMode(GL_FRONT_AND_BACK, WireframeMode ? GL_LINE : GL_FILL);
        
        glUseProgram(Shader->shaderProgramId);
        glUniformMatrix4fv(MVP_Matrix_Location, 1, GL_FALSE, glm::value_ptr(MVP_Matrix));

        glBindTexture(GL_TEXTURE_2D, TextureId);
        glBindVertexArray(VertexArrayObject);
        glDrawElements(DrawMode, NumIndices, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

}
