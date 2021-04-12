#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#include "ModelLoader.h"

int main()
{
    using namespace awesome;
    // TODO: get rid of absolute paths
    std::string modelFile{ "D:/code/OpenGLRenderer/Models/triangle.obj" };
    std::vector<Mesh> model = LoadModel(modelFile);

    return 0;
}
