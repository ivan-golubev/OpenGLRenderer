#include "ModelLoader.h"
#include "Renderer.h"

int main()
{   
    using namespace awesome;
    Renderer renderer{};

    {
        Model model1("Models/rectangle_textured.fbx", "Shaders/vertex_shader_textured.glsl", "Shaders/fragment_shader_textured.glsl");
        renderer.SubmitModel(std::move(model1));

        Model model2("Models/triangle_textured.fbx", "Shaders/vertex_shader_textured.glsl", "Shaders/fragment_shader_textured.glsl");
        renderer.SubmitModel(std::move(model2));
    }

    renderer.ExecuteRenderLoop();

    return 0;
}
