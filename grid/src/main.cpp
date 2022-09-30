#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "data/texture.h"
#include <zar/api/opengl/gl_camera.h>


#include "data/model.h"

#include "data/shader.h"
#include "objects/game_object.h"
#include "zar/api/opengl/gl_animation.h"
#include "zar/api/opengl/gl_animator.h"
#include "zar/api/opengl/gl_cube.h"
#include "zar/ecs/components/camera_component.h"

#include "utility/dev.h"
#include "data/Skybox.h"

#include "imgui.h"
#include "PxPhysXConfig.h"
#include "PxPhysicsAPI.h"
#include "data/floor.h"
#include "systems/game_object_system.h"

using namespace physx;

#define PX_FOUNDATION_VERSION_MAJOR 1
#define PX_FOUNDATION_VERSION_MINOR 0
#define PX_FOUNDATION_VERSION_BUGFIX 0

#define PX_FOUNDATION_VERSION                                                                                          \
((PX_FOUNDATION_VERSION_MAJOR << 24) + (PX_FOUNDATION_VERSION_MINOR << 16) + (PX_FOUNDATION_VERSION_BUGFIX << 8) + 0)

#define MAX_NUM_ACTOR_SHAPES 128
#define PVD_HOST "127.0.0.1"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

zar::GLCamera* camera = new zar::GLCamera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

PxDefaultAllocator g_allocator;
PxDefaultErrorCallback g_error_callback;
PxFoundation* g_foundation = NULL;
PxPhysics* g_physics = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* g_scene = NULL;
PxMaterial* g_material = NULL;
PxPvd* g_pvd = NULL;

PxRigidDynamic* create_dynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
    PxRigidDynamic* dynamic = PxCreateDynamic(*g_physics, t, geometry, *g_material, 10.0f);
    dynamic->setAngularDamping(0.5f);
    dynamic->setLinearVelocity(velocity);
    g_scene->addActor(*dynamic);
    return dynamic;
}

void create_stack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
    PxShape* shape = g_physics->createShape(PxSphereGeometry(4), *g_material);
    // PxShape* shape = g_physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *g_material);
    for (PxU32 i = 0; i < size; i++)
    {
        for (PxU32 j = 0; j < size - i; j++)
        {
            PxTransform local_tm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
            PxRigidDynamic* body = g_physics->createRigidDynamic(t.transform(local_tm));
            body->attachShape(*shape);
            PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
            g_scene->addActor(*body);
        }
    }
    shape->release();
}

void init_physics(const bool interactive)
{
    g_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, g_allocator, g_error_callback);

    g_pvd = PxCreatePvd(*g_foundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    g_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    g_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_foundation, PxTolerancesScale(), true, g_pvd);

    PxSceneDesc sceneDesc(g_physics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    g_scene = g_physics->createScene(sceneDesc);

    PxPvdSceneClient* pvdClient = g_scene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    g_material = g_physics->createMaterial(0.5f, 0.5f, 0.6f);

    PxRigidStatic* groundPlane = PxCreatePlane(*g_physics, PxPlane(0, 1, 0, 0), *g_material);
    g_scene->addActor(*groundPlane);

    // for (PxU32 i = 0; i < 2; i++)
    //     create_stack(PxTransform(PxVec3(0, 0, stack_z -= 10.0f)), 10, 2.0f);

    // if (interactive)
    create_dynamic(PxTransform(PxVec3(0, 20, 0)), PxSphereGeometry(10), PxVec3(0, -50, -1));
}

void step_physics(bool interactive)
{
    PX_UNUSED(interactive);
    g_scene->simulate(1.0f / 60.0f);
    g_scene->fetchResults(true);
}

void cleanup_physics(bool interactive)
{
    PX_UNUSED(interactive);
    g_scene->release();
    gDispatcher->release();
    g_physics->release();
    PxPvdTransport* transport = g_pvd->getTransport();
    g_pvd->release();
    transport->release();

    g_foundation->release();

    printf("SnippetHelloWorld done.\n");
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
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
    grid::dev::load_shader("camera");
    grid::dev::load_shader("animation");

    grid::Shader shader_camera = grid::dev::get_shader("camera");
    grid::GameObject vampire("assets/objects/vampire/dancing_vampire.dae", true);
    grid::GameObjectSystem* objects = grid::GameObjectSystem::instance();

    objects->add("assets/objects/bear/bear.obj");
    objects->add(&vampire);

    grid::GLSkybox* skybox = new grid::GLSkybox("blue", "png");
    grid::Floor* floor = new grid::Floor("marble.jpg", glm::vec3(100, -0.01f, 100), 50.0f);

    shader_camera.use();

    zar::CameraComponent camera_component(camera);
    camera_component.start();

    static const PxU32 frameCount = 100;
    init_physics(true);
    /*for (PxU32 i = 0; i < frameCount; i++)
        stepPhysics(false);
    cleanupPhysics(false);*/


    objects->start();

    glm::mat4 projection = camera->get_projection_matrix(
          static_cast<float>(grid::dev::SCREEN_WIDTH) / static_cast<float>(grid::dev::SCREEN_HEIGHT));
    
    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 view = camera->get_view_matrix();
        
        camera_component.update();

        auto current_frame = static_cast<float>(glfwGetTime());
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;

        processInput(window);
        objects->update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_camera.use();

        // --------------------------------------------------------------------------------------------------------

      

        shader_camera.set_mat4("projection", projection);
        shader_camera.set_mat4("view", view);


        // --------------------------------------------------------------------------------------------------------

        skybox->render(*camera, glm::vec3(1));
        floor->render(*camera, glm::vec3(1));

        objects->set_projection_view(projection, view);

        objects->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cleanup_physics(true);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->process_keyboard(zar::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->process_keyboard(zar::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->process_keyboard(zar::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->process_keyboard(zar::RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        create_dynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
        spdlog::info("create_stack");
        // createDynamic(camera, PxSphereGeometry(3.0f), camera.rotate(PxVec3(0, 0, -1)) * 200);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, const double xposIn, const double yposIn)
{
    const float xpos = static_cast<float>(xposIn);
    const float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, const double yoffset)
{
    camera->process_mouse_scroll(static_cast<float>(yoffset));
}
