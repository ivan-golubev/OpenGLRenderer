#include "Renderer.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ModelLoader.h"

namespace awesome
{
    void Renderer::CreateRenderingContext()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        RenderingContext = glfwCreateWindow(WindowWidth, WindowHeigh, "AwesomeRender", nullptr, nullptr);
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

    void Renderer::SubmitModel(Model& model)
    {
        for(auto& mesh: model.meshes)
            drawables.emplace_back(mesh, model.shaderProgram.shaderProgramId);
        /* mesh data has been passed to the GPU and is no longer needed */
        model.ClearMeshes();
    }

    void Renderer::ExecuteRenderLoop() 
    {
        while (!glfwWindowShouldClose(RenderingContext))
        {
            ProcessInput();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (auto& d : drawables)
                d.Draw();

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
