#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace awesome {

    void Camera::Tick(double deltaTimeSec, GLFWwindow* renderingContext)
    {
        static glm::vec3 const upWorldSpace = glm::vec3(0.0f, 1.0f, 0.0f);

        ProcessInput(deltaTimeSec, renderingContext);

        /* Update the camera's vectors */
        Right = glm::normalize(glm::cross(upWorldSpace, Forward));
        Up = glm::normalize(glm::cross(Forward, Right));

        ViewMatrix = glm::lookAt(Position, Position + Forward, Up);
    }

    void Camera::ProcessInput(double deltaTimeSec, GLFWwindow* renderingContext)
    {
        /* Updating the camera position */
        float deltaTimeFloat = static_cast<float>(deltaTimeSec);
        if (glfwGetKey(renderingContext, GLFW_KEY_W) == GLFW_PRESS)
            Position += Forward * CameraMoveSpeed;
        if (glfwGetKey(renderingContext, GLFW_KEY_S) == GLFW_PRESS)
            Position -= Forward * CameraMoveSpeed;
        if (glfwGetKey(renderingContext, GLFW_KEY_D) == GLFW_PRESS)
            Position -= Right * CameraMoveSpeed;
        if (glfwGetKey(renderingContext, GLFW_KEY_A) == GLFW_PRESS)
            Position += Right * CameraMoveSpeed;
    }

    void Camera::UpdateRotation(float pitchDelta, float yawDelta)
    {
        Pitch += pitchDelta;
        Yaw += yawDelta;

        /* constrain pitch and yaw */
        //if (Pitch < -89.0f)
        //    Pitch = -89.0f;
        //if (Pitch > 89.0f)
        //    Pitch = 89.0f;

        /* update the forward direction */
        glm::vec3 NewForward{
            cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
            sin(glm::radians(Pitch)),
            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))
        };
        Forward = glm::normalize(NewForward);
    }
}