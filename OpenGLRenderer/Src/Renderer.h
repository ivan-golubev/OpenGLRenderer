#pragma once

struct GLFWwindow;

using GLuint = unsigned int;

namespace awesome
{
    struct Mesh;

	class Renderer
	{
	public:
        Renderer();
        ~Renderer();

        void SubmitModel(Mesh& mesh, GLuint shaderProgramId);
        void ExecuteRenderLoop();
    private:
        void ProcessInput();
        void CreateRenderingContext();
        void LocateOpenGlFunctions();

        GLuint shaderProgramId;
        GLFWwindow* RenderingContext;
	};
}
