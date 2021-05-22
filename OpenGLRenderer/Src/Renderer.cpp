#include "Renderer.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ModelLoader.h"

namespace awesome
{
    Renderer* Renderer::GetInstance()
    {
        if (!Instance)
            Instance = new Renderer();
        return Instance;
    }

    void Renderer::DestroyInstance()
    {
        delete Instance;
        Instance = nullptr;
    }

    void Renderer::CreateRenderingContext()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        RenderingContext = glfwCreateWindow(WindowWidth, WindowHeight, "AwesomeRender", nullptr, nullptr);
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

    void MouseCallback(GLFWwindow*, double xpos, double ypos)
    {
        Renderer* renderer = Renderer::GetInstance();
        if (renderer)
            renderer->UpdateMousePosition(xpos, ypos);
    }

    void Renderer::UpdateMousePosition(double xpos, double ypos)
    {
        glm::vec2 NewMousePosition{ static_cast<float>(xpos), static_cast<float>(ypos) };

        float yawDelta = (NewMousePosition.x - MousePosition.x) * MouseSensitivity;
        /* reversed since y-coordinates go from bottom to top */
        float pitchDelta = (MousePosition.y - NewMousePosition.y) * MouseSensitivity;
        
        MousePosition = NewMousePosition;
        MainCamera.UpdateRotation(pitchDelta, yawDelta);
    }

    Renderer::Renderer()
    {
        CreateRenderingContext();
        LocateOpenGlFunctions();

        glEnable(GL_DEPTH_TEST);
        /* capture the cursor */
        glfwSetInputMode(RenderingContext, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(RenderingContext, MousePosition.x, MousePosition.y);        
        glfwSetCursorPosCallback(RenderingContext, MouseCallback);

        glfwSetFramebufferSizeCallback(RenderingContext, FramebufferSizeCallback);
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
        glm::mat4 const projectionMatrix = glm::perspective(glm::radians(45.0f), (float) WindowWidth / WindowHeight, 0.1f, 100.0f);

        while (!glfwWindowShouldClose(RenderingContext))
        {
            ProcessInput();
            double const now = glfwGetTime();
            DeltaTimeSec = now - LastFrameTimeSec;
            LastFrameTimeSec = now;

            MainCamera.Tick(DeltaTimeSec, RenderingContext);
            glm::mat4 const & viewMatrix = MainCamera.GetViewMatrix();

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
    }
}
