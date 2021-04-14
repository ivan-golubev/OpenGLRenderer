#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include "ModelLoader.h"
#include "Renderer.h"

int main(int argc, char* argv[])
{
    using namespace awesome;
    std::string modelFile{ std::filesystem::absolute("Models/triangle.obj").generic_string() };
    std::vector<Mesh> model = LoadModel(modelFile);
    
    Renderer renderer{};
    renderer.ExecuteUpdateLoop();

    return 0;
}
