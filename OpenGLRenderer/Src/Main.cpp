#include "ModelLoader.h"
#include "Renderer.h"

int main()
{   
    using namespace awesome;
    Renderer renderer{};

    Model model1( "Models/rectangle.fbx", "Shaders/vertex_shader.glsl", "Shaders/fragment_shader_blue.glsl" );
    Model model2( "Models/triangle.fbx", "Shaders/vertex_shader.glsl", "Shaders/fragment_shader_red.glsl" );

    renderer.SubmitModel(model1);
    renderer.SubmitModel(model2);

    renderer.ExecuteRenderLoop();

    return 0;
}
