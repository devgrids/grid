#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "data/texture.h"
#include <zar/api/opengl/gl_camera.h>

#include "data/application.h"
#include "data/model.h"

#include "data/shader.h"
#include "objects/game_object.h"
#include "zar/api/opengl/gl_animation.h"
#include "zar/api/opengl/gl_animator.h"
#include "zar/api/opengl/gl_cube.h"
#include "zar/ecs/components/camera_component.h"

#include "data/asset.h"
#include "data/Skybox.h"

#include "imgui.h"
#include "PxPhysXConfig.h"
#include "PxPhysicsAPI.h"
#include "data/floor.h"

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

    grid::Asset::load_shader("cubemaps");
    grid::Asset::load_shader("skybox");
    grid::Asset::load_shader("terrain");

    grid::Shader shader_camera("7.3.camera");
    grid::Shader shader_animation("1.model");
    grid::Shader shader_model("model");

    grid::GameObject object_bear("assets/objects/bear/bear.obj");
    grid::GameObject plane("assets/objects/plane/plane.obj");
    grid::Model object_vampire("assets/objects/vampire/dancing_vampire.dae");
    zar::GLAnimation dance_animation("assets/objects/vampire/dancing_vampire.dae", object_vampire.get_bone_info_map(),
                                     object_vampire.get_bone_count());
    zar::GLAnimator animator(&dance_animation);
    grid::GLSkybox* skybox = new grid::GLSkybox("nubes", "jpg");
    grid::Floor* floor = new grid::Floor("marble.jpg", glm::vec3(100, -0.01f, 100), 50.0f);


    const grid::Texture texture1("assets/textures/container.jpg");
    grid::Texture texture2("assets/textures/awesomeface.png");

    shader_camera.use();
    shader_camera.set_int("texture1", 0);
    shader_camera.set_int("texture2", 1);

    zar::CameraComponent camera_component(camera);
    camera_component.start();

    static const PxU32 frameCount = 100;
    init_physics(true);
    /*for (PxU32 i = 0; i < frameCount; i++)
        stepPhysics(false);
    cleanupPhysics(false);*/


    glm::mat4 model;

    while (!glfwWindowShouldClose(window))
    {
        camera_component.update();

        auto current_frame = static_cast<float>(glfwGetTime());
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;

        processInput(window);
        animator.update_animation(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        texture1.bind();
        glActiveTexture(GL_TEXTURE1);
        texture2.bind();

        shader_camera.use();

        // --------------------------------------------------------------------------------------------------------

        glm::mat4 projection = camera->get_projection_matrix(
            static_cast<float>(Application::SCR_WIDTH) / static_cast<float>(Application::SCR_HEIGHT));
        glm::mat4 view = camera->get_view_matrix();

        shader_camera.set_mat4("projection", projection);
        shader_camera.set_mat4("view", view);

        // --------------------------------------------------------------------------------------------------------

        skybox->render(*camera, glm::vec3(1));

        step_physics(true);

        PxScene* scene;
        PxGetPhysics().getScenes(&scene, 1);
        PxU32 nb_actors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
        if (nb_actors)
        {
            std::vector<PxRigidActor*> actors(nb_actors);
            scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC,
                             reinterpret_cast<PxActor**>(&actors[0]), nb_actors);
            //renderActors(&actors[0], static_cast<PxU32>(actors.size()));

            PxShape* shapes[MAX_NUM_ACTOR_SHAPES];

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0, 0, 0));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(10, 0, 10));

            shader_model.set_mat4("model", model);
            plane.render(shader_model, projection, view);

            for (PxU32 i = 1; i < static_cast<PxU32>(actors.size()); i++)
            {
                const PxU32 nb_shapes = actors[i]->getNbShapes();
                // spdlog::info("nbShapes: {}", actors.size());
                PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
                actors[i]->getShapes(shapes, nb_shapes);
                // bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;

                for (PxU32 j = 0; j < nb_shapes; j++)
                {
                    const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
                    PxGeometryHolder h = shapes[j]->getGeometry();

                    if (shapes[j]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                    // render object

                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(shapePose.getPosition().x,
                                                            shapePose.getPosition().y - 10.0f,
                                                            shapePose.getPosition().z));
                    // spdlog::info("position ({},{},{})", shapePose.getPosition().x,
                    //              shapePose.getPosition().y,
                    //              shapePose.getPosition().z);

                    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    shader_camera.set_mat4("model", model);

                    zar::render_cube();
                }
            }
        }

        shader_animation.use();
        shader_animation.set_mat4("projection", projection);
        shader_animation.set_mat4("view", view);

        auto transforms = animator.get_final_bone_matrices();
        for (int i = 0; i < transforms.size(); ++i)
            shader_animation.set_mat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, -0.4f, 3.0f));
        model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
        shader_animation.set_mat4("model", model);
        object_vampire.draw(shader_camera);

        object_bear.render(shader_model, projection, view);

        floor->render(*camera, glm::vec3(1));

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
