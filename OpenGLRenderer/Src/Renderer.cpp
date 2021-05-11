#include "Renderer.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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
        glEnable(GL_DEPTH_TEST);
    }

    Renderer::~Renderer()
    {
        glfwTerminate();
    }

    void Renderer::SubmitModel(Model&& model)
    {
        for(auto& mesh: model.meshes)
            drawables.emplace_back(mesh, model.shaderProgram);
    }

    void Renderer::ExecuteRenderLoop() 
    {
        glm::mat4 const modelMatrix = glm::mat4(1.f);
        glm::mat4 const projectionMatrix = glm::perspective(glm::radians(45.0f), (float) WindowWidth / WindowHeigh, 0.1f, 100.0f);

        while (!glfwWindowShouldClose(RenderingContext))
        {
            ProcessInput();
            glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), CameraPosition);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
            for (auto& d : drawables)
                d.Draw(mvpMatrix);

            glfwSwapBuffers(RenderingContext);
            glfwPollEvents();
        }
    }

    void Renderer::ProcessInput()
    {
        if (glfwGetKey(RenderingContext, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(RenderingContext, true);
        
        const float cameraMoveSpeed = 0.005f;
        if (glfwGetKey(RenderingContext, GLFW_KEY_W) == GLFW_PRESS)
            CameraPosition.z += cameraMoveSpeed;
        if (glfwGetKey(RenderingContext, GLFW_KEY_S) == GLFW_PRESS)
            CameraPosition.z -= cameraMoveSpeed;
        if (glfwGetKey(RenderingContext, GLFW_KEY_D) == GLFW_PRESS)
            CameraPosition.x += cameraMoveSpeed;
        if (glfwGetKey(RenderingContext, GLFW_KEY_A) == GLFW_PRESS)
            CameraPosition.x -= cameraMoveSpeed;
    }
}
