#include "zar/API/GLCube.h"

// #include "spdlog/spdlog.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


int main()
{
    GLCube* c = new GLCube();

    std::cout << c->getNumber();

    // spdlog::info("Welcome to spdlog!");
    // std::cout << "VERSION: " << GRID_VERSION << "\n";
    //

    return 0;
}
