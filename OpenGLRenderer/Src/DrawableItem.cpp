#include "DrawableItem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
        glGenVertexArrays(1, &VertexArrayObject);
        glGenBuffers(1, &VertexBufferObject);
        glGenBuffers(1, &ElementArrayBufferObject);

        glBindVertexArray(VertexArrayObject);

        /* pass the vertex data */
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);                
        glBufferData(GL_ARRAY_BUFFER, mesh.VerticesSize(), mesh.Vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, Vertex::TOTAL_VERTEX_ATTRIBS, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

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
        VertexBufferObject = other.VertexBufferObject;
        ElementArrayBufferObject = other.ElementArrayBufferObject;
        VertexArrayObject = other.VertexArrayObject;
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

    void DrawableItem::Draw()
    {
        /* set the wireframe mode */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glUseProgram(ShaderProgramId);
        glBindVertexArray(VertexArrayObject);
        glDrawElements(DrawMode, NumIndices, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

}
