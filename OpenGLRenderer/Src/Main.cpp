#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include "ModelLoader.h"
#include "ShaderProgram.h"
#include "Renderer.h"

int main()
{   
    using namespace awesome;
    Renderer renderer{};
    {
        std::string modelFile{ std::filesystem::absolute("Models/triangle.obj").generic_string() };
        std::vector<Mesh> model = LoadModel(modelFile);

        std::string vertexShaderFile{ std::filesystem::absolute("Shaders/vertex_shader.glsl").generic_string() };
        std::string fragmentShaderFile{ std::filesystem::absolute("Shaders/fragment_shader.glsl").generic_string() };

        ShaderProgram shaderProgram{ vertexShaderFile, fragmentShaderFile };
        if (shaderProgram.LinkingComplete())
            renderer.SubmitModel(model[0], shaderProgram.shaderProgramId);
        else {
            std::cout << "Failed to initialize the shaders" << std::endl;
            exit(-1);
        }
    }
    renderer.ExecuteRenderLoop();

    return 0;
}
