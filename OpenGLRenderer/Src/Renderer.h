#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "DrawableItem.h"

struct GLFWwindow;

using GLuint = unsigned int;

namespace awesome
{
    struct Model;

	class Renderer
	{
	public:
        Renderer();
        ~Renderer();

        void SubmitModel(Model&& model);
        void ExecuteRenderLoop();
    private:
        void ProcessInput();
        void CreateRenderingContext();
        void LocateOpenGlFunctions();

        GLFWwindow* RenderingContext;
        std::vector<DrawableItem> drawables{};

        glm::vec3 CameraPosition{ 0.0f, 0.0f, -2.0f };

        int WindowWidth{ 800 };
        int WindowHeigh{ 600 };
	};
}
