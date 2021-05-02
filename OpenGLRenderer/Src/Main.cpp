#include "ModelLoader.h"
#include "Renderer.h"

int main()
{   
    using namespace awesome;
    Renderer renderer{};

    Model model1( "Models/triangle1.obj", "Shaders/vertex_shader.glsl", "Shaders/fragment_shader_red.glsl" );
    Model model2( "Models/triangle2.obj", "Shaders/vertex_shader.glsl", "Shaders/fragment_shader_blue.glsl" );

    renderer.SubmitModel(model1);
    renderer.SubmitModel(model2);

    renderer.ExecuteRenderLoop();

    return 0;
}
