#pragma once

#include <vector>
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

        int WindowWidth{ 800 };
        int WindowHeigh{ 600 };
	};
}
