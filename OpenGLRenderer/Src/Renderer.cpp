#include "Renderer.h"

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ModelLoader.h"

namespace awesome
{
    void Renderer::CreateRenderingContext()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        RenderingContext = glfwCreateWindow(800, 600, "AwesomeRender", nullptr, nullptr);
        if (!RenderingContext)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(RenderingContext);
    }

    void Renderer::LocateOpenGlFunctions()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }
    }

    void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    Renderer::Renderer()
    {
        CreateRenderingContext();
        LocateOpenGlFunctions();
        glfwSetFramebufferSizeCallback(RenderingContext, FramebufferSizeCallback);
    }

    Renderer::~Renderer()
    {
        glfwTerminate();
    }

    void Renderer::SubmitModel(Mesh& mesh, GLuint shaderProgramId)
    {
        this->shaderProgramId = shaderProgramId;
        unsigned int VertexBuffer;
        glGenBuffers(1, &VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
        auto vertices = mesh.vertices.data();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    void Renderer::ExecuteRenderLoop() 
    {
        while (!glfwWindowShouldClose(RenderingContext))
        {
            ProcessInput();

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgramId);

            glfwSwapBuffers(RenderingContext);
            glfwPollEvents();
        }
    }

    void Renderer::ProcessInput()
    {
        if (glfwGetKey(RenderingContext, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(RenderingContext, true);
    }
}
