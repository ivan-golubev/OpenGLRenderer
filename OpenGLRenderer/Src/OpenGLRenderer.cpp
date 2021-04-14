#include <vector>
#include <string>
#include <iostream>

#include "ModelLoader.h"
#include "Renderer.h"

int main()
{
    using namespace awesome;
    // TODO: get rid of absolute paths
    std::string modelFile{ "D:/code/OpenGLRenderer/Models/triangle.obj" };
    std::vector<Mesh> model = LoadModel(modelFile);
    Renderer renderer{};
    renderer.Init();
    renderer.StartUpdateLoop();

    return 0;
}
