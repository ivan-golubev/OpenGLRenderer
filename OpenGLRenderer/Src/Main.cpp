#include "ModelLoader.h"
#include "Renderer.h"

int main()
{   
    using namespace awesome;
    Renderer* renderer{ Renderer::GetInstance()};

    {
        Model model2("Models/King.fbx", "Shaders/vertex_shader_textured.glsl", "Shaders/fragment_shader_textured.glsl");
        renderer->SubmitModel(std::move(model2));
    }

    renderer->ExecuteRenderLoop();

    /* terminating the renderer */
    Renderer::DestroyInstance();
    return 0;
}
