#pragma once

struct GLFWwindow;

namespace awesome
{
	class Renderer
	{
	public:
        Renderer();
        ~Renderer();

        void ExecuteUpdateLoop();
    private:
        void CreateRenderingContext();
        void LocateOpenGlFunctions();

        GLFWwindow* RenderingContext;
	};
}
