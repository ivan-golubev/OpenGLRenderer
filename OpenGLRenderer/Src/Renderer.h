#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "DrawableItem.h"
#include "Camera.h"

struct GLFWwindow;

using GLuint = unsigned int;

namespace awesome
{
    struct Model;    

	class Renderer
	{
	public:
        /* Singleton */
        static Renderer* GetInstance();
        static void DestroyInstance();
        
        void SubmitModel(Model&& model);
        void ExecuteRenderLoop();
        
        Renderer(Renderer& other) = delete;
        void operator=(const Renderer&) = delete;

    private:
        /* note that this renderer instance is resident in memory */
        static inline Renderer* Instance{ nullptr };
        Renderer();
        ~Renderer();

        void ProcessInput();
        void CreateRenderingContext();
        void LocateOpenGlFunctions();
        void UpdateMousePosition(double xpos, double ypos);

        GLFWwindow* RenderingContext;
        std::vector<DrawableItem> drawables{};
        Camera MainCamera{};

        int WindowWidth{ 800 };
        int WindowHeight{ 600 };
        double LastFrameTimeSec;
        double DeltaTimeSec;

        /* TODO: there should be a better place to store this, outside of the Renderer class */
        const float MouseSensitivity{ 0.1f };
        /* center of the screen */
        glm::vec2 MousePosition{ WindowWidth / 2.0f, WindowHeight / 2.0f };

        friend void MouseCallback(GLFWwindow*, double xpos, double ypos);
	};
}
