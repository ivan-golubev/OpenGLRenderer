#pragma once

struct GLFWwindow;

namespace awesome
{
	class Renderer
	{
	public:
        void Init();
        void StartUpdateLoop();
    private:
        void CreateRenderingContext();
        void LocateOpenGlFunctions();

        GLFWwindow* RenderingContext;
	};
}
