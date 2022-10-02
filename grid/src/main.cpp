#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <zar/api/opengl/gl_camera.h>

#include "objects/game_object.h"
#include "zar/api/opengl/gl_animation.h"

#include "zar/ecs/components/camera_component.h"

#include "utility/dev.h"
#include "data/skybox.h"

#include "imgui.h"

#include "data/floor.h"
#include "systems/game_object_system.h"
#include "systems/physics_system.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);


zar::GLCamera* camera = new zar::GLCamera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = grid::dev::SCREEN_WIDTH / 2.0f;
float last_y = grid::dev::SCREEN_HEIGHT / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(grid::dev::SCREEN_WIDTH, grid::dev::SCREEN_HEIGHT, "Grid", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (GLenum err = glewInit()) return 0;
    glEnable(GL_DEPTH_TEST);

    grid::dev::load_shader("cubemaps");
    grid::dev::load_shader("skybox");
    grid::dev::load_shader("terrain");
    grid::dev::load_shader("model");
    grid::dev::load_shader("animation");

    grid::GameObject_System* objects = grid::GameObject_System::instance();
    grid::Physics_System* physics_system = grid::Physics_System::instance();
    // physics_system->init_physics(true);

    grid::GameObject vampire(
        "assets/objects/vampire/dancing_vampire.dae",
        glm::vec3(10.0f, 0.0f, 10.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        grid::ANIMATION,
        grid::SPHERE
    );

    objects->add(
        "assets/objects/bear/bear.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        grid::MODEL,
        grid::SPHERE
    );

    objects->add(&vampire);

    grid::GLSkybox* skybox = new grid::GLSkybox("blue", "png");
    grid::Floor* floor = new grid::Floor("marble.jpg", glm::vec3(100, -0.01f, 100), 50.0f);

    zar::CameraComponent camera_component(camera);
    camera_component.start();


    objects->start();

    const glm::mat4 projection = camera->get_projection_matrix(
        static_cast<float>(grid::dev::SCREEN_WIDTH) / static_cast<float>(grid::dev::SCREEN_HEIGHT));

    while (!glfwWindowShouldClose(window))
    {
        const glm::mat4 view = camera->get_view_matrix();

        camera_component.update();

        const auto current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window);
        objects->update(delta_time);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // --------------------------------------------------------------------------------------------------------

        skybox->render(*camera, glm::vec3(1));
        floor->render(*camera, glm::vec3(1));

        physics_system->step_physics();

        objects->set_projection_view(projection, view);
        objects->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    physics_system->cleanup_physics();
    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->process_keyboard(zar::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->process_keyboard(zar::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->process_keyboard(zar::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->process_keyboard(zar::RIGHT, delta_time);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, const double xposIn, const double yposIn)
{
    const float xpos = static_cast<float>(xposIn);
    const float ypos = static_cast<float>(yposIn);

    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top

    last_x = xpos;
    last_y = ypos;

    camera->process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, const double yoffset)
{
    camera->process_mouse_scroll(static_cast<float>(yoffset));
}
