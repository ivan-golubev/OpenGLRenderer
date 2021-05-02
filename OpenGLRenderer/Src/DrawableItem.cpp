#include "DrawableItem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ModelLoader.h"

namespace awesome
{
	DrawableItem::DrawableItem(Mesh& mesh, GLuint shaderProgId): 
        ShaderProgramId(shaderProgId),
        VertexBufferObject(0),
        VertexArrayObject(0),
        DrawMode(GL_TRIANGLES)
	{
        glGenVertexArrays(1, &VertexArrayObject);
        glGenBuffers(1, &VertexBufferObject);

        glBindVertexArray(VertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
                
        glBufferData(GL_ARRAY_BUFFER, mesh.SizeInBytes(), mesh.Vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, Vertex::TOTAL_VERTEX_ATTRIBS, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}

    DrawableItem::DrawableItem(DrawableItem&& other) noexcept
    {
        ShaderProgramId = other.ShaderProgramId;
        VertexBufferObject = other.VertexBufferObject;
        VertexArrayObject = other.VertexArrayObject;
        DrawMode = other.DrawMode;

        other.ShaderProgramId = 0;
        other.VertexBufferObject = 0;
        other.VertexArrayObject = 0;
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
        glUseProgram(ShaderProgramId);
        glBindVertexArray(VertexArrayObject);

        glDrawArrays(DrawMode, 0, 3);
        glBindVertexArray(0);
    }

}
