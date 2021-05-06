#include "DrawableItem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "ModelLoader.h"

namespace awesome
{
	DrawableItem::DrawableItem(Mesh& mesh, GLuint shaderProgId): 
        ShaderProgramId(shaderProgId),
        VertexBufferObject(0),
        ElementArrayBufferObject(0),
        VertexArrayObject(0),
        DrawMode(GL_TRIANGLES),
        NumIndices(mesh.GetNumIndices())
	{
        /* get shader variable locations */
        TransformLocation = glGetUniformLocation(ShaderProgramId, "Transform");

        glGenVertexArrays(1, &VertexArrayObject);
        glGenBuffers(1, &VertexBufferObject);
        glGenBuffers(1, &VertexColorBufferObject);
        glGenBuffers(1, &ElementArrayBufferObject);

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

        /* unbind the VAO first, then the buffers */
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

    DrawableItem::DrawableItem(DrawableItem&& other) noexcept
    {
        ShaderProgramId = other.ShaderProgramId;
        TransformLocation = other.TransformLocation;
        VertexBufferObject = other.VertexBufferObject;
        ElementArrayBufferObject = other.ElementArrayBufferObject;
        VertexArrayObject = other.VertexArrayObject;
        VertexColorBufferObject = other.VertexColorBufferObject;
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
    }

    void DrawableItem::Draw(glm::mat4& transform)
    {
        /* set the wireframe mode */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glUseProgram(ShaderProgramId);
        glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VertexArrayObject);
        glDrawElements(DrawMode, NumIndices, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

}
